# @UnrealClaude Script
# @Description: Dump BP_Glass runtime collision, bounds, and attachment state for floating diagnosis.
import unreal

def dump(actor, prefix):
    print(prefix, actor.get_name())
    comps = actor.get_components_by_class(unreal.PrimitiveComponent)
    for c in comps:
        parent = c.get_attach_parent()
        pname = parent.get_name() if parent else 'None'
        try:
            ce = c.get_collision_enabled()
        except Exception as e:
            ce = 'ERR:' + str(e)
        try:
            prof = c.get_collision_profile_name()
        except Exception as e:
            prof = 'ERR:' + str(e)
        try:
            sim = c.get_editor_property('body_instance').get_editor_property('b_simulate_physics')
        except Exception:
            sim = 'n/a'
        try:
            mesh = c.get_editor_property('static_mesh')
            mesh_name = mesh.get_name() if mesh else 'None'
        except Exception:
            mesh_name = 'n/a'
        try:
            o, e = c.get_local_bounds()
            bounds = 'origin=' + str(o) + ' extent=' + str(e)
        except Exception as ex:
            bounds = 'ERR:' + str(ex)
        print('  ', c.get_name(), 'parent=', pname, 'collision=', ce, 'profile=', prof, 'sim=', sim, 'mesh=', mesh_name, bounds)

actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors()
for a in actors:
    if a.get_class().get_name() == 'BP_Glass_C':
        dump(a, 'LEVEL')
