# @UnrealClaude Script
# @Description: Verify BaseColour parameters on glass coffee materials.
import unreal
mel = unreal.MaterialEditingLibrary
for p in ['/Game/Objects/Coffee/Glass/M_GlassCoffeeMiddle', '/Game/Objects/Coffee/Glass/M_GlassCoffeeTop']:
    mat = unreal.load_asset(p)
    print('MAT', p)
    print('  vectors', [str(x) for x in mel.get_vector_parameter_names(mat)])
    node = mel.get_material_property_input_node(mat, unreal.MaterialProperty.MP_BASE_COLOR)
    print('  base', node.get_class().get_name() if node else None, node.get_editor_property('parameter_name') if node and node.get_class().get_name() == 'MaterialExpressionVectorParameter' else '')
