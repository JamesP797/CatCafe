# Project Assets Domain — CatCafe

## Skeletons

2 skeletons:
- `SK_Mannequin` — `/Game/Characters/Player/SK_Mannequin` (player Manny/Quinn meshes and player animations).
- `SKEL_Cat` — `/Game/Characters/Cat/SKEL_Cat` (cat mesh and cat animations).

## Content Structure

Content/
  Characters/     — 74 assets (player Manny/Quinn meshes, cat mesh, 2 skeletons, 3 AnimBPs, player/cat animations, textures, physics assets, ControlRigs)
  Objects/        — 121 assets (coffee machine, cup, grinder, portafilter, milk jug/carton, tamper, knock box, steam wand, coin, chairs, doors, tables, lamps, street lights, building materials)
  __ExternalActors__ — 297 World Partition actor assets
  __ExternalObjects__ — 15 World Partition object assets
  Input/          — 9 assets (4 InputActions, 2 InputMappingContexts, 3 touch UI/interface assets)
  Level/          — `Lvl_FirstPerson` map + `BP_FirstPersonGameMode`
  UI/             — 2 assets (`WBP_HUD`, `T_InteractIcon_D`)
  Lighting/       — 3 textures (`T_Black`, `T_WarmWhite_D`, `T_Money_Pouch_D`)
  Root materials  — 2 masters (`M_Master_Flat`, `M_Master_Transparent`) + 11 material instances (gold, floor, wood, metal, ceramic, cat fur/eye, wall, stone, rubber, carton)
  Collections/, Developers/, UnrealClaude/ — empty

## Key Paths

- Player body mesh: `/Game/Characters/Player/SKM_Manny_Simple`
- Player animations: `/Game/Characters/Player/Anims/` (Attack, Jog, Jump, Walk subfolders)
- Player blend space: `/Game/Characters/Player/Anims/BS_Idle_Walk_Run`
- Cat mesh: `/Game/Characters/Cat/SK_Cat`
- Cat animations: `/Game/Characters/Cat/` (`A_Cat_*`, `CatArmature_Drink`)
- Coin blueprint: `/Game/Objects/Coin/BP_Coin`
- Coin material: `/Game/MI_Gold`
- Money pouch texture: `/Game/Lighting/T_Money_Pouch_D`
- Main map: `/Game/Level/Lvl_FirstPerson`
