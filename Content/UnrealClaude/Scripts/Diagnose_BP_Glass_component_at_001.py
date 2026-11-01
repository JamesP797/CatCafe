/**
 * @UnrealClaude Script
 * @Description: Dump BP_Glass component attachment hierarchy and transforms for diagnosing contents components.
 */
import unreal

bp = unreal.load_asset('/Game/Objects/Coffee/Glass/BP_Glass')
cls = bp.generated_class()
cdo = unreal.get_default_object(cls)

def dump(actor, prefix):
    print(prefix, actor.get_name(), actor.get_class().get_name())
    try:
        comps = actor.get_components_by_class(unreal.SceneComponent)
    except Exception as e:
        print('component query failed', e)
        return
    for c in comps:
        parent = c.get_attach_parent()
        pname = parent.get_name() if parent else 'None'
        try:
            rel = c.get_relative_transform()
        except Exception:
            rel = None
        print('  ', c.get_name(), 'class=', c.get_class().get_name(), 'parent=', pname, 'rel=', rel)

dump(cdo, 'CDO')

actors = []
try:
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors()
except Exception:
    actors = unreal.EditorLevelLibrary.get_all_level_actors()
for a in actors:
    if a.get_class().get_name() == 'BP_Glass_C':
        dump(a, 'LEVEL')
