# @UnrealClaude Script
# @Description: Read current BaseColor constants from glass coffee materials.
import unreal
mel = unreal.MaterialEditingLibrary
for p in ['/Game/Objects/Coffee/Glass/M_GlassCoffeeMiddle', '/Game/Objects/Coffee/Glass/M_GlassCoffeeTop']:
    mat = unreal.load_asset(p)
    node = mel.get_material_property_input_node(mat, unreal.MaterialProperty.MP_BASE_COLOR)
    print('MAT', p, 'node', node.get_class().get_name() if node else None)
    if node:
        try:
            print('  constant', node.get_editor_property('constant'))
        except Exception as e:
            print('  no constant', e)
