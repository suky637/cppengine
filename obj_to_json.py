import json

def parse_obj_file(file_path):
    vertices = []
    texture_coords = []
    indices = []

    with open(file_path, 'r') as file:
        for line in file:
            data = line.strip().split()

            if not data:
                continue

            if data[0] == 'v':
                # Vertex data: v x y z
                vertex = list(map(float, data[1:]))
                vertices.append(vertex)
            elif data[0] == 'vt':
                # Texture coordinate data: vt u v
                tex_coord = list(map(float, data[1:]))
                texture_coords.append(tex_coord)
            elif data[0] == 'f':
                # Face data: f v1/vt1 v2/vt2 v3/vt3
                face = []
                tex_indices = []
                for vertex in data[1:]:
                    v_index, t_index, _ = vertex.split('/')
                    face.append(int(v_index) - 1)  # Convert to 0-based indexing
                    tex_indices.append(int(t_index) - 1)  # Convert to 0-based indexing

                indices.extend(face)

    return vertices, texture_coords, indices

# Replace 'your_file.obj' with your .obj file path
FILE_NAME = 'cube'
obj_file_path = FILE_NAME + '.obj'

vertices, texture_coords, indices = parse_obj_file(obj_file_path)

JSONDATA = []
OBJ = {}
OBJ["Vertices"] = vertices
OBJ["Indices"] = indices
OBJ["UVs"] = texture_coords
OBJ["Textures"] = []
OBJ["Transforms"] = []
JSONDATA.append(OBJ)

# Write to a pretty-formatted JSON file
with open(FILE_NAME + ".json", 'w') as file_stream:
    json.dump(JSONDATA, file_stream, indent=4)
