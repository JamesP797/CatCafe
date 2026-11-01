# @UnrealClaude Script
# @Description: Dump material graph nodes and base-color inputs for the glass coffee materials.
import unreal
mel = unreal.MaterialEditingLibrary

def dump(mat_path):
    mat = unreal.load_asset(mat_path)
    print('MATERIAL', mat_path)
    try:
        base = mel.get_material_property_input_node(mat, unreal.MaterialProperty.MP_BASE_COLOR)
        print('  BaseColor node:', base.get_class().get_name() if base else None, base.get_name() if base else '')
    except Exception as e:
        print('  BaseColor query failed:', e)
    for e in mel.get_material_expressions(mat):
        cls = e.get_class().get_name()
        name = ''
        try:
            name = str(e.get_editor_property('parameter_name'))
        except Exception:
            pass
        print('  NODE', cls, e.get_name(), 'param=', name)

dump('/Game/Objects/Coffee/Glass/M_GlassCoffeeMiddle')
dump('/Game/Objects/Coffee/Glass/M_GlassCoffeeTop')
