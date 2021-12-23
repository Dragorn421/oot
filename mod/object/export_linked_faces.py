import bpy
import bmesh
import mathutils
import sys

argv = sys.argv
if not '--' in argv:
	print('Cannot export: No export arguments')
	sys.exit(1)

argv = argv[argv.index('--') + 1:]
object_name = argv[0]
out_file_name = argv[1]

def fail(msg):
    print('fail:', msg)
    raise Exception(msg)

object = bpy.data.objects[object_name]
if object.type != 'MESH':
    fail('Not a mesh object')

mesh = object.data

bm = bmesh.new()
bm.from_mesh(mesh)

bmesh.ops.triangulate(bm, faces=bm.faces)

geometry = dict()
for face in bm.faces:
    if len(face.verts) != 3:
        fail(f'Found face with {len(face.verts)} verts instead of 3 (after triangulation)')
    v1 = face.verts[0]
    v2 = face.verts[1]
    v3 = face.verts[2]
    e1 = bm.edges.get((v1,v2))
    e2 = bm.edges.get((v2,v3))
    e3 = bm.edges.get((v3,v1))
    nf = [None, None, None]
    for i, e in enumerate((e1,e2,e3)):
        edgeNeighbours = []
        for otherFace in bm.faces:
            if otherFace != face and e in otherFace.edges:
                edgeNeighbours.append(otherFace)
        if len(edgeNeighbours) > 1:
            fail(f'Found face with {len(edgeNeighbours)} neighbour faces')
        nf[i] = edgeNeighbours[0] if edgeNeighbours else None
    geometry[face] = {
        'index': len(geometry),
        'verts': [v1, v2, v3],
        'normal': face.normal,
        'neighbours': nf,
    }

items = [None] * len(geometry)

# Blender coords to OoT coords
global_mat = mathutils.Matrix(((1,0,0),(0,0,1),(0,-1,0))).to_4x4()

for face in geometry.values():
    verts_co = [global_mat @ v.co for v in face['verts']]
    verts = ',\n            '.join(f'{{{co.x}f, {co.y}f, {co.z}f}}' for co in verts_co)
    norm_co = global_mat @ face['normal']
    normal = f"{{{norm_co.x}f, {norm_co.y}f, {norm_co.z}f}}"
    neighbours = ', '.join(('-1' if nf is None else str(geometry[nf]['index'])) for nf in face['neighbours'])
    items[face['index']] = '{{' + verts + '},\n           ' + normal + ',\n           {' + neighbours + '}}'

with open(out_file_name, 'w', newline='\n') as f:
    f.write(',\n'.join(f"/* {i:03} */ {item}" for i, item in enumerate(items)))
    f.write('\n')

bm.free()
