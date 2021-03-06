/*
 * Copyright (C) 2005-2008 MaNGOS <http://www.mangosproject.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef DBCSTRUCTURE_H
#define DBCSTRUCTURE_H

#include "Platform/Define.h"

#include <map>
#include <set>
#include <vector>

// Structures using to access raw DBC data and required packing to portability

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push,N), also any gcc version not support it at some platform
#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

enum AreaTeams
{
    AREATEAM_NONE  = 0,
    AREATEAM_ALLY  = 2,
    AREATEAM_HORDE = 4
};

enum AreaFlags
{
    AREA_FLAG_SNOW             = 0x00000001,                // snow (only Dun Morogh, Naxxramas, Razorfen Downs and Winterspring)
    AREA_FLAG_UNK1             = 0x00000002,                // unknown, (only Naxxramas and Razorfen Downs)
    AREA_FLAG_UNK2             = 0x00000004,                // Only used on development map
    AREA_FLAG_SLAVE_CAPITAL    = 0x00000008,                // slave capital city flag?
    AREA_FLAG_UNK3             = 0x00000010,                // unknown
    AREA_FLAG_SLAVE_CAPITAL2   = 0x00000020,                // slave capital city flag?
    AREA_FLAG_UNK4             = 0x00000040,                // many zones have this flag
    AREA_FLAG_ARENA            = 0x00000080,                // arena, both instanced and world arenas
    AREA_FLAG_CAPITAL          = 0x00000100,                // main capital city flag
    AREA_FLAG_CITY             = 0x00000200,                // only for one zone named "City" (where it located?)
    AREA_FLAG_OUTLAND          = 0x00000400,                // outland zones? (only Eye of the Storm not have this flag, but have 0x00004000 flag)
    AREA_FLAG_SANCTUARY        = 0x00000800,                // sanctuary area (PvP disabled)
    AREA_FLAG_NEED_FLY         = 0x00001000,                // only Netherwing Ledge, Socrethar's Seat, Tempest Keep, The Arcatraz, The Botanica, The Mechanar, Sorrow Wing Point, Dragonspine Ridge, Netherwing Mines, Dragonmaw Base Camp, Dragonmaw Skyway
    AREA_FLAG_UNUSED1          = 0x00002000,                // not used now (no area/zones with this flag set in 2.4.2)
    AREA_FLAG_OUTLAND2         = 0x00004000,                // outland zones? (only Circle of Blood Arena not have this flag, but have 0x00000400 flag)
    AREA_FLAG_PVP              = 0x00008000,                // pvp objective area? (Death's Door also has this flag although it's no pvp object area)
    AREA_FLAG_ARENA_INSTANCE   = 0x00010000,                // used by instanced arenas only
    AREA_FLAG_UNUSED2          = 0x00020000,                // not used now (no area/zones with this flag set in 2.4.2)
    AREA_FLAG_UNK5             = 0x00040000,                // just used for Amani Pass, Hatchet Hills
    AREA_FLAG_LOWLEVEL         = 0x00100000                 // used for some starting areas with area_level <=15
};

struct AreaTableEntry
{
    uint32    ID;                                           // 0
    uint32    mapid;                                        // 1
    uint32    zone;                                         // 2 if 0 then it's zone, else it's zone id of this area
    uint32    exploreFlag;                                  // 3, main index
    uint32    flags;                                        // 4, unknown value but 312 for all cities
                                                            // 5-9 unused
    int32     area_level;                                   // 10
    char*     area_name[16];                                // 11-26
                                                            // 27, string flags, unused
    uint32    team;                                         // 28
};

struct AreaTriggerEntry
{
    uint32    id;                                           // 0
    uint32    mapid;                                        // 1
    float     x;                                            // 2
    float     y;                                            // 3
    float     z;                                            // 4
    float     radius;                                       // 5
    float     box_x;                                        // 6 extent x edge
    float     box_y;                                        // 7 extent y edge
    float     box_z;                                        // 8 extent z edge
    float     box_orientation;                              // 9 extent rotation by about z axis
};

struct BankBagSlotPricesEntry
{
    uint32      ID;
    uint32      price;
};

struct BattlemasterListEntry
{
    uint32      id;                                         // 0
    uint32      mapid[3];                                   // 1-3 mapid
                                                            // 4-8 unused
    uint32      type;                                       // 9 (3 - BG, 4 - arena)
    uint32      minlvl;                                     // 10
    uint32      maxlvl;                                     // 11
    uint32      maxplayersperteam;                          // 12
                                                            // 13-14 unused
    char*       name[16];                                   // 15-30
                                                            // 31 string flag, unused
                                                            // 32 unused
};

struct CharTitlesEntry
{
    uint32      ID;                                         // 0, title ids, for example in Quest::GetCharTitleId()
    //uint32      unk1;                                     // 1 flags?
    //char*       name[16];                                 // 2-17, unused
                                                            // 18 string flag, unused
    //char*       name2[16];                                // 19-34, unused
                                                            // 35 string flag, unused
    uint32      bit_index;                                  // 36 used in PLAYER_CHOSEN_TITLE and 1<<(index-1) in PLAYER__FIELD_KNOWN_TITLES
};

struct ChatChannelsEntry
{
    uint32      ChannelID;                                  // 0
    uint32      flags;                                      // 1
    char*       pattern[16];                                // 3-18
                                                            // 19 string flags, unused
    //char*       name[16];                                 // 20-35 unused
                                                            // 36 string flag, unused
};

struct ChrClassesEntry
{
    uint32      ClassID;                                    // 0
                                                            // 1-2, unused
    uint32      powerType;                                  // 3
                                                            // 4, unused
    //char*       name[16];                                 // 5-20 unused
                                                            // 21 string flag, unused
    //char*       string1[16];                              // 21-36 unused
                                                            // 37 string flag, unused
    //char*       string2[16];                              // 38-53 unused
                                                            // 54 string flag, unused
                                                            // 55, unused
    uint32 spellfamily;                                     // 56
                                                            // 57, unused
};

struct ChrRacesEntry
{
    uint32      RaceID;                                     // 0
                                                            // 1 unused
    uint32      FactionID;                                  // 2 facton template id
                                                            // 3 unused
    uint32      model_m;                                    // 4
    uint32      model_f;                                    // 5
                                                            // 6-7 unused
    uint32      TeamID;                                     // 8 (7-Alliance 1-Horde)
                                                            // 9-12 unused
    uint32      startmovie;                                 // 13 id from CinematicCamera.dbc
    char*       name[16];                                   // 14-29 used for DBC language detection/selection
                                                            // 30 string flags, unused
    //char*       string1[16];                              // 31-46 used for DBC language detection/selection
                                                            // 47 string flags, unused
    //char*       string2[16];                              // 48-63 used for DBC language detection/selection
                                                            // 64 string flags, unused
                                                            // 65-67 unused
    //uint32    addon                                       // 68 (0 - original race, 1 - tbc addon, ...) unused
};

struct CreatureDisplayInfoEntry
{
    uint32      Displayid;                                  // 0
                                                            // 1-3,unused
    float       scale;                                      // 4
                                                            // 5-13,unused
};

struct CreatureFamilyEntry
{
    uint32    ID;                                           // 0
                                                            // 1 bitmask, unused    minScale
                                                            // 2 0/1 unused         minScaleLevel
                                                            // 3 bitmask, unused    maxScale
                                                            // 4 0/60, unused       maxScaleLevel
                                                            // 5 skillID, unused
    uint32    tamable;                                      // 6 if this = 270 then "is tamable Creature" (or 0 is non-tamable); skillID2
    uint32    petFoodMask;                                  // 7
    char*     Name[16];                                     // 8-23
                                                            // 24 string flags, unused
                                                            // 25 icon, unused
};

struct DurabilityCostsEntry
{
    uint32    Itemlvl;                                      // 0
    uint32    multiplier[29];                               // 1-29
};

struct DurabilityQualityEntry
{
    uint32    Id;                                           // 0
    float     quality_mod;                                  // 1
};

struct EmotesTextEntry
{
    uint32    Id;
    uint32    textid;
};

struct FactionEntry
{
    uint32      ID;                                         // 0
    int32       reputationListID;                           // 1
    uint32      BaseRepRaceMask[4];                         // 2-5 Base reputation race masks (see enum Races)
    uint32      BaseRepClassMask[4];                        // 6-9 Base reputation class masks (see enum Classes)
    int32       BaseRepValue[4];                            // 10-13 Base reputation values
    uint32      ReputationFlags[4];                         // 14-17 Default flags to apply
    uint32      team;                                       // 18 enum Team
    char*       name[16];                                   // 19-34
                                                            // 35 string flags, unused
    //char*     description[16];                            // 36-51 unused
                                                            // 52 string flags, unused
};

enum FactionMasks
{
    FACTION_MASK_PLAYER   = 1,                              // any player
    FACTION_MASK_ALLIANCE = 2,                              // player or creature from alliance team
    FACTION_MASK_HORDE    = 4,                              // player or creature from horde team
    FACTION_MASK_MONSTER  = 8                               // aggressive creature from monster team
                                                            // if none flags set then non-aggressive creature
};

struct FactionTemplateEntry
{
    uint32      ID;                                         // 0
    uint32      faction;                                    // 1
                                                            // 2 not used
    uint32      ourMask;                                    // 3 if mask set (see FactionMasks) then faction included in masked team
    uint32      friendlyMask;                               // 4 if mask set (see FactionMasks) then faction friendly to masked team
    uint32      hostileMask;                                // 5 if mask set (see FactionMasks) then faction hostile to masked team
    uint32      enemyFaction1;                              // 6
    uint32      enemyFaction2;                              // 7
    uint32      enemyFaction3;                              // 8
    uint32      enemyFaction4;                              // 9
    uint32      friendFaction1;                             // 10
    uint32      friendFaction2;                             // 11
    uint32      friendFaction3;                             // 12
    uint32      friendFaction4;                             // 13
    //-------------------------------------------------------  end structure

    // helpers
    bool IsFriendlyTo(FactionTemplateEntry const& entry) const
    {
        if(enemyFaction1  == entry.faction || enemyFaction2  == entry.faction || enemyFaction3 == entry.faction || enemyFaction4 == entry.faction )
            return false;
        if(friendFaction1 == entry.faction || friendFaction2 == entry.faction || friendFaction3 == entry.faction || friendFaction4 == entry.faction )
            return true;
        return (friendlyMask & entry.ourMask) != 0;
    }
    bool IsHostileTo(FactionTemplateEntry const& entry) const
    {
        if(enemyFaction1  == entry.faction || enemyFaction2  == entry.faction || enemyFaction3 == entry.faction || enemyFaction4 == entry.faction )
            return true;
        if(friendFaction1 == entry.faction || friendFaction2 == entry.faction || friendFaction3 == entry.faction || friendFaction4 == entry.faction )
            return false;
        return (hostileMask & entry.ourMask) != 0;
    }
    bool IsHostileToPlayers() const { return hostileMask & FACTION_MASK_PLAYER; }
    bool IsNeutralToAll() const { return hostileMask == 0 && friendlyMask == 0 && enemyFaction1==0 && enemyFaction2==0 && enemyFaction3==0 && enemyFaction4==0; }
};

struct GemPropertiesEntry
{
    uint32      ID;
    uint32      spellitemenchantement;
    uint32      color;
};

#define GT_MAX_LEVEL    100
struct GtCombatRatingsEntry
{
    float    ratio;
};

struct GtChanceToMeleeCritBaseEntry
{
    float    base;
};

struct GtChanceToMeleeCritEntry
{
    float    ratio;
};

struct GtChanceToSpellCritBaseEntry
{
    float    base;
};

struct GtChanceToSpellCritEntry
{
    float    ratio;
};

struct GtOCTRegenHPEntry
{
    float    ratio;
};

//struct GtOCTRegenMPEntry
//{
//    float    ratio;
//};

struct GtRegenHPPerSptEntry
{
    float    ratio;
};

struct GtRegenMPPerSptEntry
{
    float    ratio;
};

struct ItemDisplayInfoEntry
{
    uint32      ID;
    uint32      randomPropertyChance;
};

//struct ItemCondExtCostsEntry
//{
//    uint32      ID;
//    uint32      condExtendedCost;                           // ItemPrototype::CondExtendedCost
//    uint32      itemextendedcostentry;                      // ItemPrototype::ExtendedCost
//    uint32      arenaseason;                                // arena season number(1-4)
//};

struct ItemExtendedCostEntry
{
    uint32      ID;                                         // 0 extended-cost entry id
    uint32      reqhonorpoints;                             // 1 required honor points
    uint32      reqarenapoints;                             // 2 required arena points
    uint32      reqitem[5];                                 // 3-7 required item id
    uint32      reqitemcount[5];                            // 8-12 required count of 1st item
    uint32      reqpersonalarenarating;                     // 13 required personal arena rating
};

struct ItemRandomPropertiesEntry
{
    uint32    ID;                                           // 0
    //char*     internalName                                // 1   unused
    uint32    enchant_id[3];                                // 2-4
                                                            // 5-6 unused, 0 only values, reserved for additional enchantments?
    //char*     nameSuffix[16]                              // 7-22, unused
                                                            // 23 nameSufix flags, unused
};

struct ItemRandomSuffixEntry
{
    uint32    ID;                                           // 0
    //char*     name[16]                                    // 1-16 unused
                                                            // 17, name flags, unused
                                                            // 18  unused
    uint32    enchant_id[3];                                // 19-21
    uint32    prefix[3];                                    // 22-24
};

struct ItemSetEntry
{
    //uint32    id                                          // 0 item set ID
    char*     name[16];                                     // 1-16
                                                            // 17 string flags, unused
                                                            // 18-28 items from set, but not have all items listed, use ItemPrototype::ItemSet instead
                                                            // 29-34 unused
    uint32    spells[8];                                    // 35-42
    uint32    items_to_triggerspell[8];                     // 43-50
    uint32    required_skill_id;                            // 51
    uint32    required_skill_value;                         // 52
};

struct LockEntry
{
    uint32      ID;                                         // 0
    uint32      keytype[5];                                 // 1-5
                                                            // 6-8, not used
    uint32      key[5];                                     // 9-13
                                                            // 14-16, not used
    uint32      requiredminingskill;                        // 17
    uint32      requiredlockskill;                          // 18
                                                            // 19-32, not used
};

enum MapTypes
{
    MAP_COMMON          = 0,
    MAP_INSTANCE        = 1,
    MAP_RAID            = 2,
    MAP_BATTLEGROUND    = 3,
    MAP_ARENA           = 4
};

struct MapEntry
{
    uint32      MapID;                                      // 0
    //char*       internalname;                             // 1 unused
    uint32      map_type;                                   // 2
                                                            // 3 unused
    char*       name[16];                                   // 4-19
                                                            // 20 name flags, unused
                                                            // 21-23 unused (something PvPZone related - levels?)
                                                            // 24-26
    uint32      linked_zone;                                // 27 common zone for instance and continent map
    //char*     hordeIntro                                  // 28-43 text for PvP Zones
                                                            // 44 intro text flags
    //char*     allianceIntro                               // 45-60 text for PvP Zones
                                                            // 46 intro text flags
                                                            // 47-61 not used
    uint32      multimap_id;                                // 62
                                                            // 63-65 not used
    //chat*     unknownText1                                // 66-81 unknown empty text fields, possible normal Intro text.
                                                            // 82 text flags
    //chat*     heroicIntroText                             // 83-98 heroic mode requirement text
                                                            // 99 text flags
    //chat*     unknownText2                                // 100-115 unknown empty text fields
                                                            // 116 text flags
    int32       parent_map;                                 // 117 map_id of parent map
    //float start_x                                         // 118 enter x coordinate (if exist single entry)
    //float start_y                                         // 119 enter y coordinate (if exist single entry)
    uint32 resetTimeRaid;                                   // 120
    uint32 resetTimeHeroic;                                 // 121
                                                            // 122-123
    uint32      addon;                                      // 124 (0-original maps,1-tbc addon)

    // Helpers
    bool IsExpansionMap() const { return addon != 0; }
    bool IsDungeon() const { return map_type == MAP_INSTANCE || map_type == MAP_RAID; }
    bool SupportsHeroicMode() const { return resetTimeHeroic && !resetTimeRaid; }
    bool HasResetTime() const { return resetTimeHeroic || resetTimeRaid; }
};

struct QuestSortEntry
{
    uint32      id;                                         // 0, sort id
    //char*       name[16];                                 // 1-16, unused
                                                            // 17 name flags, unused
};

struct RandomPropertiesPointsEntry
{
    //uint32  Id;                                           // 0 hidden key
    uint32    itemLevel;                                    // 1
    uint32    EpicPropertiesPoints[5];                      // 2-6
    uint32    RarePropertiesPoints[5];                      // 7-11
    uint32    UncommonPropertiesPoints[5];                  // 12-16
};

//struct SkillLineCategoryEntry{
//    uint32    id;                                           // 0 hidden key
//    char*     name[16];                                     // 1 - 17 Category name
//                                                                  // 18 string flag
//    uint32    displayOrder;                                 // Display order in character tab
//};

//struct SkillRaceClassInfoEntry{
//    uint32    id;                                           // 0
//    uint32    skillId;                                      // 1 present some refrences to unknown skill
//    uint32    raceMask;                                     // 2
//    uint32    classMask;                                    // 3
//    uint32    flags;                                        // 4 mask for some thing
//    uint32    reqLevel;                                     // 5
//    uint32    skillTierId;                                  // 6
//    uint32    skillCostID;                                  // 7
//};

//struct SkillTiersEntry{
//    uint32    id;                                           // 0
//    uint32    skillValue[16];                               // 1-17 unknown possibly add value on learn?
//    uint32    maxSkillValue[16];                            // Max value for rank
//};

struct SkillLineEntry
{
    uint32    id;                                           // 0
    uint32    categoryId;                                   // 1 (index from SkillLineCategory.dbc)
    //uint32    skillCostID;                                // 2 not used
    char*     name[16];                                     // 3-18
                                                            // 19 string flags, not used
    //char*     description[16];                            // 20-35, not used
                                                            // 36 string flags, not used
    uint32    spellIcon;                                    // 37
};

enum AbilytyLearnType
{
    ABILITY_LEARNED_ON_GET_PROFESSION_SKILL     = 1,
    ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL  = 2
};

struct SkillLineAbilityEntry
{
    uint32    id;                                           // 0, INDEX
    uint32    skillId;                                      // 1
    uint32    spellId;                                      // 2
    uint32    racemask;                                     // 3
    uint32    classmask;                                    // 4
    //uint32    racemaskNot;                                // 5 always 0 in 2.4.2
    //uint32    classmaskNot;                               // 6 always 0 in 2.4.2
    uint32    req_skill_value;                              // 7 for trade skill.not for training.
    uint32    forward_spellid;                              // 8
    uint32    learnOnGetSkill;                              // 9 can be 1 or 2 for spells learned on get skill
    uint32    max_value;                                    // 10
    uint32    min_value;                                    // 11
                                                            // 12-13, unknown, always 0
    uint32    reqtrainpoints;                               // 14
};

struct SpellEntry
{
    uint32    Id;                                           // 0 normally counted from 0 field (but some tools start counting from 1, check this before tool use for data view!)
    uint32    Category;                                     // 1
    //uint32     castUI                                     // 2 not used
    uint32    Dispel;                                       // 3
    uint32    Mechanic;                                     // 4
    uint32    Attributes;                                   // 5
    uint32    AttributesEx;                                 // 6
    uint32    AttributesEx2;                                // 7
    uint32    AttributesEx3;                                // 8
    uint32    AttributesEx4;                                // 9
    //uint32    AttributesEx5;                              // 10 not used
    //uint32    AttributesEx6;                              // 11 not used
    uint32    Stances;                                      // 12
    uint32    StancesNot;                                   // 13
    uint32    Targets;                                      // 14
    uint32    TargetCreatureType;                           // 15
    uint32    RequiresSpellFocus;                           // 16
    //uint32    FacingCasterFlags;                          // 17 not used
    uint32    CasterAuraState;                              // 18
    uint32    TargetAuraState;                              // 19
    //uint32    CasterAuraStateNot                          // 20 not used
    //uint32    TargetAuraStateNot                          // 21 not used
    uint32    CastingTimeIndex;                             // 22
    uint32    RecoveryTime;                                 // 23
    uint32    CategoryRecoveryTime;                         // 24
    uint32    InterruptFlags;                               // 25
    uint32    AuraInterruptFlags;                           // 26
    uint32    ChannelInterruptFlags;                        // 27
    uint32    procFlags;                                    // 28
    uint32    procChance;                                   // 29
    uint32    procCharges;                                  // 30
    uint32    maxLevel;                                     // 31
    uint32    baseLevel;                                    // 32
    uint32    spellLevel;                                   // 33
    uint32    DurationIndex;                                // 34
    int32     powerType;                                    // 35
    uint32    manaCost;                                     // 36
    uint32    manaCostPerlevel;                             // 37
    uint32    manaPerSecond;                                // 38
    uint32    manaPerSecondPerLevel;                        // 39
    uint32    rangeIndex;                                   // 40
    float     speed;                                        // 41
    uint32    modalNextSpell;                               // 42
    uint32    StackAmount;                                  // 43
    uint32    Totem[2];                                     // 44-45
    int32     Reagent[8];                                   // 46-53
    uint32    ReagentCount[8];                              // 54-61
    int32     EquippedItemClass;                            // 62 (value)
    int32     EquippedItemSubClassMask;                     // 63 (mask)
    int32     EquippedItemInventoryTypeMask;                // 64 (mask)
    uint32    Effect[3];                                    // 65-67
    int32     EffectDieSides[3];                            // 68-70
    uint32    EffectBaseDice[3];                            // 71-73
    float     EffectDicePerLevel[3];                        // 74-76
    float     EffectRealPointsPerLevel[3];                  // 77-79
    int32     EffectBasePoints[3];                          // 80-82 (don't must be used in spell/auras explicitly, must be used cached Spell::m_currentBasePoints)
    uint32    EffectMechanic[3];                            // 83-85
    uint32    EffectImplicitTargetA[3];                     // 86-88
    uint32    EffectImplicitTargetB[3];                     // 89-91
    uint32    EffectRadiusIndex[3];                         // 92-94 - spellradius.dbc
    uint32    EffectApplyAuraName[3];                       // 95-97
    uint32    EffectAmplitude[3];                           // 98-100
    float     EffectMultipleValue[3];                       // 101-103
    uint32    EffectChainTarget[3];                         // 104-106
    uint32    EffectItemType[3];                            // 107-109
    int32     EffectMiscValue[3];                           // 110-112
    uint32    EffectTriggerSpell[3];                        // 113-115
    float     EffectPointsPerComboPoint[3];                 // 116-118
    uint32    SpellVisual;                                  // 119
                                                            // 120 not used
    uint32    SpellIconID;                                  // 121
    uint32    activeIconID;                                 // 122
    uint32    spellPriority;                                // 123
    char*     SpellName[16];                                // 124-139
    //uint32    SpellNameFlag;                              // 140
    char*     Rank[16];                                     // 141-156
    //uint32    RankFlags;                                  // 157
    //char*     Description[16];                            // 158-173 not used
    //uint32    DescriptionFlags;                           // 174     not used
    //char*     ToolTip[16];                                // 175-190 not used
    //uint32    ToolTipFlags;                               // 191     not used
    uint32    ManaCostPercentage;                           // 192
    uint32    StartRecoveryCategory;                        // 193
    uint32    StartRecoveryTime;                            // 194
    uint32    MaxTargetLevel;                               // 195
    uint32    SpellFamilyName;                              // 196
    uint64    SpellFamilyFlags;                             // 197+198
    uint32    MaxAffectedTargets;                           // 199
    uint32    DmgClass;                                     // 200  defenseType
    uint32    PreventionType;                               // 201
    //uint32    StanceBarOrder;                             // 202 not used
    float     DmgMultiplier[3];                             // 203-205
    //uint32    MinFactionId;                               // 206 not used, and 0 in 2.4.2
    //uint32    MinReputation;                              // 207 not used, and 0 in 2.4.2
    //uint32    RequiredAuraVision;                         // 208 not used
    uint32    TotemCategory[2];                             // 209-210
    uint32    AreaId;                                       // 211
    uint32    SchoolMask;                                   // 212 school mask

    private:
        // prevent creating custom entries (copy data from original infact)
        SpellEntry(SpellEntry const&);                      // DON'T must have implementation
};

typedef std::set<uint32> SpellCategorySet;
typedef std::map<uint32,SpellCategorySet > SpellCategoryStore;

struct SpellCastTimesEntry
{
    uint32    ID;
    uint32    CastTime;
};

struct SpellFocusObjectEntry
{
    uint32    ID;                                           // 0
    //char*     Name[16];                                   // 1-15 unused
                                                            // 16 string flags, unused
};

// stored in SQL table
struct SpellThreatEntry
{
    uint32      spellId;
    int32       threat;
};

struct SpellRadiusEntry
{
    uint32    ID;
    float     Radius;
    float     Radius2;
};

struct SpellRangeEntry
{
    uint32    ID;
    float     minRange;
    float     maxRange;
};

struct SpellShapeshiftEntry
{
    uint32 ID;                                              // 0
    //uint32 buttonPosition;                                // 1 unused
    //char*  Name[16];                                      // 2-17 unused
    //uint32 NameFlags;                                     // 18 unused
    uint32 flags1;                                          // 19
    int32  creatureType;                                    // 20 <=0 humanoid, other normal creature types
    //uint32 unk1;                                          // 21 unused
    uint32 attackSpeed;                                     // 22
    //uint32 modelID;                                       // 23 unused, alliance modelid (where horde case?)
    //uint32 unk2;                                          // 24 unused
    //uint32 unk3;                                          // 25 unused
    //uint32 unk4;                                          // 26 unused
    //uint32 unk5;                                          // 27 unused
    //uint32 unk6;                                          // 28 unused
    //uint32 unk7;                                          // 29 unused
    //uint32 unk8;                                          // 30 unused
    //uint32 unk9;                                          // 31 unused
    //uint32 unk10;                                         // 32 unused
    //uint32 unk11;                                         // 33 unused
    //uint32 unk12;                                         // 34 unused
};

struct SpellDurationEntry
{
    uint32    ID;
    int32     Duration[3];
};

enum ItemEnchantmentType
{
    ITEM_ENCHANTMENT_TYPE_NONE         = 0,
    ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL = 1,
    ITEM_ENCHANTMENT_TYPE_DAMAGE       = 2,
    ITEM_ENCHANTMENT_TYPE_EQUIP_SPELL  = 3,
    ITEM_ENCHANTMENT_TYPE_RESISTANCE   = 4,
    ITEM_ENCHANTMENT_TYPE_STAT         = 5,
    ITEM_ENCHANTMENT_TYPE_TOTEM        = 6
};

struct SpellItemEnchantmentEntry
{
    uint32      ID;                                         // 0
    uint32      type[3];                                    // 1-3
    uint32      amount[3];                                  // 4-6
    //uint32    amount2[3]                                  // 7-9 always same as similar `amount` value
    uint32      spellid[3];                                 // 10-12
    char*       description[16];                            // 13-29
                                                            // 30 description flags
    uint32      aura_id;                                    // 31
    uint32      slot;                                       // 32
    uint32      GemID;                                      // 33
    uint32      EnchantmentCondition;                       // 34
};

struct SpellItemEnchantmentConditionEntry
{
    uint32  ID;
    uint8   Color[5];
    uint8   Comparator[5];
    uint8   CompareColor[5];
    uint32  Value[5];
};

struct StableSlotPricesEntry
{
    uint32 Slot;
    uint32 Price;
};

struct TalentEntry
{
    uint32    TalentID;                                     // 0
    uint32    TalentTab;                                    // 1 index in TalentTab.dbc (TalentTabEntry)
    uint32    Row;                                          // 2
    uint32    Col;                                          // 3
    uint32    RankID[5];                                    // 4-8
                                                            // 9-12 not used, always 0, maybe not used high ranks
    uint32    DependsOn;                                    // 13 index in Talent.dbc (TalentEntry)
                                                            // 14-15 not used
    uint32    DependsOnRank;                                // 16
                                                            // 17-20 not used
};

struct TalentTabEntry
{
    uint32    TalentTabID;                                  // 0
    //char*   name[16];                                     // 1-16, unused
    //uint32  nameFlags;                                    // 17, unused
    //unit32  spellicon;                                    // 18
                                                            // 19 not used
    uint32    ClassMask;                                    // 20
    uint32    tabpage;                                      // 21
    //char*   internalname;                                 // 22
};

struct TaxiNodesEntry
{
    uint32    ID;                                           // 0
    uint32    map_id;                                       // 1
    float     x;                                            // 2
    float     y;                                            // 3
    float     z;                                            // 4
    //char*     name[16];                                   // 5-21
                                                            // 22 string flags, unused
    uint32    horde_mount_type;                             // 23
    uint32    alliance_mount_type;                          // 24
};

enum TotemCategoryType
{
    TOTEM_CATEGORY_TYPE_KNIFE   = 1,
    TOTEM_CATEGORY_TYPE_TOTEM   = 2,
    TOTEM_CATEGORY_TYPE_ROD     = 3,
    TOTEM_CATEGORY_TYPE_PICK    = 21,
    TOTEM_CATEGORY_TYPE_STONE   = 22,
    TOTEM_CATEGORY_TYPE_HAMMER  = 23,
    TOTEM_CATEGORY_TYPE_SPANNER = 24
};

struct TotemCategoryEntry
{
    uint32    ID;                                           // 0
    //char*   name[16];                                     // 1-16
                                                            // 17 string flags, unused
    uint32    categoryType;                                 // 18 (one for specialization)
    uint32    categoryMask;                                 // 19 (compatibility mask for same type: different for totems, compatible from high to low for rods)
};

struct WorldMapAreaEntry
{
    //uint32    ID;                                         // 0
    uint32    map_id;                                       // 1
    uint32    area_id;                                      // 2 index (continent 0 areas ignored)
    //char*   internal_name                                 // 3
    float     y1;                                           // 4
    float     y2;                                           // 5
    float     x1;                                           // 6
    float     x2;                                           // 7
    int32   virtual_map_id;                                 // 8 -1 (map_id have correct map) other: virtual map where zone show (map_id - where zone in fact internally)
};

struct WorldSafeLocsEntry
{
    uint32    ID;                                           // 0
    uint32    map_id;                                       // 1
    float     x;                                            // 2
    float     y;                                            // 3
    float     z;                                            // 4
    //char*   name[16]                                      // 5-20 name, unused
                                                            // 21 name flags, unused
};

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some platform
#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

// Structures not used for casting to loaded DBC data and not required then packing
struct TalentSpellPos
{
    TalentSpellPos() : talent_id(0), rank(0) {}
    TalentSpellPos(uint16 _talent_id, uint8 _rank) : talent_id(_talent_id), rank(_rank) {}

    uint16 talent_id;
    uint8  rank;
};

typedef std::map<uint32,TalentSpellPos> TalentSpellPosMap;

struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0),price(0) {}
    TaxiPathBySourceAndDestination(uint32 _id,uint32 _price) : ID(_id),price(_price) {}

    uint32    ID;
    uint32    price;
};
typedef std::map<uint32,TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32,TaxiPathSetForSource> TaxiPathSetBySource;

struct TaxiPathNode
{
    TaxiPathNode() : mapid(0), x(0),y(0),z(0),actionFlag(0),delay(0) {}
    TaxiPathNode(uint32 _mapid, float _x, float _y, float _z, uint32 _actionFlag, uint32 _delay) : mapid(_mapid), x(_x),y(_y),z(_z),actionFlag(_actionFlag),delay(_delay) {}

    uint32    mapid;
    float     x;
    float     y;
    float     z;
    uint32    actionFlag;
    uint32    delay;
};
typedef std::vector<TaxiPathNode> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

#define TaxiMaskSize 8
typedef uint32 TaxiMask[TaxiMaskSize];
#endif
