# @UnrealClaude Script
# @Description: Add a BaseColour vector parameter to the glass coffee middle/top materials.
import unreal
mel = unreal.MaterialEditingLibrary
for p in ['/Game/Objects/Coffee/Glass/M_GlassCoffeeMiddle', '/Game/Objects/Coffee/Glass/M_GlassCoffeeTop']:
    mat = unreal.load_asset(p)
    base = mel.get_material_property_input_node(mat, unreal.MaterialProperty.MP_BASE_COLOR)
    color = unreal.LinearColor(0.078125, 0.054137, 0.039876, 1.0)
    if base:
        try:
            color = base.get_editor_property('constant')
        except Exception:
            pass
    param = mel.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -400, 0)
    param.set_editor_property('parameter_name', 'BaseColour')
    param.set_editor_property('default_value', color)
    mel.connect_material_property(param, '', unreal.MaterialProperty.MP_BASE_COLOR)
    mel.recompile_material(mat)
    unreal.EditorAssetLibrary.save_asset(p, only_if_is_dirty=True)
    print('UPDATED', p, 'default', color)
