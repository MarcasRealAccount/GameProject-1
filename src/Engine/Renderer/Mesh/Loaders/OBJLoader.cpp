//
//	Created by MarcasRealAccount on 18. Nov. 2020.
//

#include "Engine/Renderer/Mesh/Loaders/OBJLoader.h"
#include "Engine/Utility/Logger.h"

#include <filesystem>
#include <string_view>

namespace gp1 {

	namespace meshLoaders {

		Logger OBJLoaderLogger = Logger("OBJLoader");

		uint64_t SplitString(const std::string_view& str, uint64_t offset, char delimiter, std::string_view* arr, uint64_t count) {
			uint64_t i = 0;
			for (; i < count; i++) {
				uint64_t start = str.find_first_not_of(delimiter, offset);
				uint64_t end = str.find_first_of(delimiter, start);
				arr[i] = str.substr(start, end - start);
				offset = end + 1;
			}
			if (i < count) {
				arr[i] = str.substr(str.find_first_not_of(delimiter, offset));
				i++;
			}
			return i;
		}

		bool ParseOBJLine(const std::string_view& line, std::vector<glm::fvec3>& positions, std::vector<glm::fvec3>& normals, std::vector<glm::fvec2>& uvs, std::vector<glm::uvec3>& indices) {
			std::string_view properLine = line;
			std::string_view splittedString[3];
			uint64_t count = 0;
			switch (properLine[0]) {
			case 'v': // Vertex specifier.
				properLine = properLine.substr(1);
				switch (properLine[0]) {
				case 'n': // Vertex normal specifier.
					count = SplitString(properLine, 1, ' ', splittedString, 3);
					if (count < 3) return false;

					normals.push_back({ static_cast<float>(atof(splittedString[0].data())), static_cast<float>(atof(splittedString[1].data())), static_cast<float>(atof(splittedString[2].data())) });
					break;
				case 't': // Vertex texture specifier.
					count = SplitString(properLine, 1, ' ', splittedString, 2);
					if (count < 2) return false;

					uvs.push_back({ static_cast<float>(atof(splittedString[0].data())), static_cast<float>(atof(splittedString[1].data())) });
					break;
				case 'p': // Vertex parameter specifier.
					break;
				case ' ': // Vertex specifier. (' ' to make sure no other types is accepted)
					count = SplitString(properLine, 1, ' ', splittedString, 3);
					if (count < 3) return false;

					positions.push_back({ static_cast<float>(atof(splittedString[0].data())), static_cast<float>(atof(splittedString[1].data())), static_cast<float>(atof(splittedString[2].data())) });
					break;
				}
				break;
			case 'f': // Face specifier.
				count = SplitString(properLine, 1, ' ', splittedString, 3);
				if (count < 3) return false;

				for (uint8_t i = 0; i < 3; i++) {
					uint32_t iv, it, in;

					if (splittedString[i].find("//") < splittedString[i].length()) {
						std::string_view indice[2];
						uint64_t c = SplitString(splittedString[i], 0, '/', indice, 2);
						if (c < 2) return false;

						iv = static_cast<uint32_t>(atoll(indice[0].data()));
						it = 0;
						in = static_cast<uint32_t>(atoll(indice[1].data()));
					} else {
						std::string_view indice[3];
						uint64_t c = SplitString(splittedString[i], 0, '/', indice, 3);
						if (c > 2) {
							iv = static_cast<uint32_t>(atoll(indice[0].data()));
							it = static_cast<uint32_t>(atoll(indice[1].data()));
							in = static_cast<uint32_t>(atoll(indice[2].data()));
						} else if (c > 1) {
							iv = static_cast<uint32_t>(atoll(indice[0].data()));
							it = static_cast<uint32_t>(atoll(indice[1].data()));
							in = 0;
						} else if (c > 0) {
							iv = static_cast<uint32_t>(atoll(indice[0].data()));
							it = 0;
							in = 0;
						} else {
							return false;
						}
					}

					if (iv == 0) {
						return false;
					}

					indices.push_back({ iv, it, in });
				}
				break;
			}
			return true;
		}

		bool LoadOBJFile(StaticMesh& mesh, const std::string& fileName) {
			FILE* file = fopen(fileName.c_str(), "r");
			if (!file) {
				OBJLoaderLogger.LogError("Failed to read file: %s", fileName.c_str());
				return false;
			}

			fseek(file, 0, SEEK_END);
			uint64_t length = ftell(file);
			if (length == 0) {
				fclose(file);
				return false;
			}
			std::string str = std::string(length, '\0');
			fseek(file, 0, SEEK_SET);
			length = fread(str.data(), sizeof(char), length, file);
			fclose(file);

			std::vector<glm::fvec3> positions;
			std::vector<glm::fvec3> normals;
			std::vector<glm::fvec2> uvs;
			std::vector<glm::uvec3> indices;

			bool isOk = true;
			uint64_t lineNumber = 0;
			uint64_t offset = 0;
			uint64_t i;
			while ((i = str.find_first_of('\n', offset)) < str.length()) {
				std::string_view line = str;
				line = line.substr(offset, i - offset);

				if (!ParseOBJLine(line, positions, normals, uvs, indices)) {
					OBJLoaderLogger.LogError("Format error on line %d: %s", lineNumber, line.data());
					isOk = false;
				}

				offset = i + 1;
				lineNumber++;
			}

			if (offset < str.length()) {
				std::string_view line = str;
				line = line.substr(offset);

				if (!ParseOBJLine(line, positions, normals, uvs, indices)) {
					OBJLoaderLogger.LogError("Format error on line %d: %s", lineNumber, line.data());
					isOk = false;
				}
			}

			if (isOk) {
				mesh.m_Vertices.clear();
				mesh.m_Indices.clear();

				std::vector<std::pair<uint32_t, glm::uvec3&>> usedIndices;

				uint32_t curIndex = 0;
				for (uint32_t index = 0; index < indices.size(); index++) {
					uint32_t actualIndex = 0;
					bool found = false;
					glm::uvec3& indice = indices[index];
					for (uint32_t preIndex = 0; preIndex < curIndex; preIndex++) {
						auto& otherIndice = usedIndices[preIndex];
						if (otherIndice.second == indice) {
							found = true;
							actualIndex = otherIndice.first;
							break;
						}
					}

					if (!found) {
						StaticMeshVertex vertex;
						vertex.position = positions[indice.x - 1ULL];
						if (indice.z > 0)
							vertex.normal = normals[indice.z - 1ULL];
						if (indice.y > 0)
							vertex.uv = uvs[indice.y - 1ULL];

						mesh.m_Vertices.push_back(vertex);
						actualIndex = curIndex++;
						usedIndices.push_back({ actualIndex, indice });
					}
					mesh.m_Indices.push_back(actualIndex);
				}
			}
			return isOk;
		}

	} // namespace meshLoaders

} // namespace gp1
