#pragma once
#include <fstream>
#include "Math/Math.h"
#include "DataTypes.h"

namespace dae
{
	namespace Utils
	{
		//Just parses vertices and indices
#pragma warning(push)
#pragma warning(disable : 4505) //Warning unreferenced local function
		static bool ParseOBJ(const std::string& filename, std::vector<VS_INPUT>& vertices, std::vector<uint32_t>& indices, bool flipAxisAndWinding = true)
		{
			std::ifstream file(filename);
			if (!file)
				return false;

			std::vector<Vector3> positions{};
			std::vector<Vector3> normals{};
			std::vector<Vector2> UVs{};

			vertices.clear();
			indices.clear();

			std::string sCommand;
			// start a while iteration ending when the end of file is reached (ios::eof)
			while (!file.eof())
			{
				//read the first word of the string, use the >> operator (istream::operator>>) 
				file >> sCommand;
				//use conditional statements to process the different commands	
				if (sCommand == "#")
				{
					// Ignore Comment
				}
				else if (sCommand == "v")
				{
					//Vertex
					float x, y, z;
					file >> x >> y >> z;

					positions.emplace_back(x, y, z);
				}
				else if (sCommand == "vt")
				{
					// Vertex TexCoord
					float u, v;
					file >> u >> v;
					UVs.emplace_back(u, 1 - v);
				}
				else if (sCommand == "vn")
				{
					// Vertex Normal
					float x, y, z;
					file >> x >> y >> z;

					normals.emplace_back(x, y, z);
				}
				else if (sCommand == "f")
				{
					//if a face is read:
					//construct the 3 vertices, add them to the vertex array
					//add three indices to the index array
					//add the material index as attibute to the attribute array
					//
					// Faces or triangles
					VS_INPUT vertex{};
					size_t iPosition, iTexCoord, iNormal;

					uint32_t tempIndices[3];
					for (size_t iFace = 0; iFace < 3; iFace++)
					{
						// OBJ format uses 1-based arrays
						file >> iPosition;
						vertex.position[0] = positions[iPosition - 1].x;
						vertex.position[1] = positions[iPosition - 1].y;
						vertex.position[2] = positions[iPosition - 1].z;

						if ('/' == file.peek())//is next in buffer ==  '/' ?
						{
							file.ignore();//read and ignore one element ('/')

							if ('/' != file.peek())
							{
								// Optional texture coordinate
								file >> iTexCoord;
								vertex.uv[0] = UVs[iTexCoord - 1].x;
								vertex.uv[1] = UVs[iTexCoord - 1].y;
							}

							if ('/' == file.peek())
							{
								file.ignore();

								// Optional vertex normal
								file >> iNormal;
								vertex.normal[0] = normals[iNormal - 1].x;
								vertex.normal[1] = normals[iNormal - 1].y;
								vertex.normal[2] = normals[iNormal - 1].z;
							}
						}

						vertices.push_back(vertex);
						tempIndices[iFace] = uint32_t(vertices.size()) - 1;
						//indices.push_back(uint32_t(vertices.size()) - 1);
					}

					indices.push_back(tempIndices[0]);
					if (flipAxisAndWinding) 
					{
						indices.push_back(tempIndices[2]);
						indices.push_back(tempIndices[1]);
					}
					else
					{
						indices.push_back(tempIndices[1]);
						indices.push_back(tempIndices[2]);
					}
				}
				//read till end of line and ignore all remaining chars
				file.ignore(1000, '\n');
			}

			//Cheap Tangent Calculations
			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				uint32_t index0 = indices[i];
				uint32_t index1 = indices[size_t(i) + 1];
				uint32_t index2 = indices[size_t(i) + 2];
				
				const Vector3& p0 { vertices[index0].position[0],vertices[index0].position[1] ,vertices[index0].position[2] };
				const Vector3& p1 { vertices[index1].position[0],vertices[index1].position[1] ,vertices[index1].position[2] };
				const Vector3& p2 { vertices[index2].position[0],vertices[index2].position[1] ,vertices[index2].position[2] };
				const Vector2& uv0 = { vertices[index0].uv[0],vertices[index0].uv[1] };
				const Vector2& uv1 = { vertices[index1].uv[0],vertices[index1].uv[1] };
				const Vector2& uv2 = { vertices[index2].uv[0],vertices[index2].uv[1] };
				
				const Vector3 edge0 = p1 - p0;
				const Vector3 edge1 = p2 - p0;
				const Vector2 diffX = Vector2(uv1.x - uv0.x, uv2.x - uv0.x);
				const Vector2 diffY = Vector2(uv1.y - uv0.y, uv2.y - uv0.y);
				float r = 1.f / Vector2::Cross(diffX, diffY);
				
				Vector3 tangent = (edge0 * diffY.y - edge1 * diffY.x) * r;
				vertices[index0].tangent[0] += tangent.x;
				vertices[index0].tangent[1] += tangent.y;
				vertices[index0].tangent[2] += tangent.z;

				vertices[index1].tangent[0] += tangent.x;
				vertices[index1].tangent[1] += tangent.y;
				vertices[index1].tangent[2] += tangent.z;

				vertices[index2].tangent[0] += tangent.x;
				vertices[index2].tangent[1] += tangent.y;
				vertices[index2].tangent[2] += tangent.z;
			}

			//Create the Tangents (reject)
			for (auto& v : vertices)
			{
				Vector3 tangent{ v.tangent[0],v.tangent[1] ,v.tangent[2] };
				Vector3 normal{ v.normal[0],v.normal[1] ,v.normal[2] };
				auto t = Vector3::Reject(tangent, normal).Normalized();
				v.tangent[0] = t.x;
				v.tangent[1] = t.y;
				v.tangent[2] = t.z;


				if(flipAxisAndWinding)
				{
					v.position[2] *= -1.f;
					v.normal[2] *= -1.f;
					v.tangent[2] *= -1.f;
				}

			}

			return true;
		}
#pragma warning(pop)

		inline float Remap(float v, float n0, float n1) {
			if (v < n1 && v > n0) {
				return (v - n0) / (n1 - n0);
			}
			else {
				return 0;
			}
		}
	}
}