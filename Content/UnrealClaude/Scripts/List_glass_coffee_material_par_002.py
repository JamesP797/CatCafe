# @UnrealClaude Script
# @Description: List scalar and vector parameters on the glass coffee materials.
import unreal
mel = unreal.MaterialEditingLibrary
for p in ['/Game/Objects/Coffee/Glass/M_GlassCoffeeMiddle', '/Game/Objects/Coffee/Glass/M_GlassCoffeeTop']:
    mat = unreal.load_asset(p)
    print('MAT', p)
    print('  scalars', [str(x) for x in mel.get_scalar_parameter_names(mat)])
    print('  vectors', [str(x) for x in mel.get_vector_parameter_names(mat)])
    print('  textures', [str(x) for x in mel.get_texture_parameter_names(mat)])
