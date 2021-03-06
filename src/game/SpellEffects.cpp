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

#include "Common.h"
#include "SharedDefines.h"
#include "Database/DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "UpdateMask.h"
#include "World.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Player.h"
#include "SkillExtraItems.h"
#include "Unit.h"
#include "CreatureAI.h"
#include "Spell.h"
#include "DynamicObject.h"
#include "SpellAuras.h"
#include "Group.h"
#include "UpdateData.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "CellImpl.h"
#include "SharedDefines.h"
#include "Pet.h"
#include "GameObject.h"
#include "GossipDef.h"
#include "Creature.h"
#include "Totem.h"
#include "CreatureAI.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "BattleGround.h"
#include "BattleGroundAV.h"
#include "BattleGroundAB.h"
#include "BattleGroundEY.h"
#include "BattleGroundWS.h"
#include "VMapFactory.h"
#include "Language.h"
#include "SocialMgr.h"
#include "Util.h"

pEffect SpellEffects[TOTAL_SPELL_EFFECTS]=
{
    &Spell::EffectNULL,                                     //  0
    &Spell::EffectInstaKill,                                //  1 SPELL_EFFECT_INSTAKILL
    &Spell::EffectSchoolDMG,                                //  2 SPELL_EFFECT_SCHOOL_DAMAGE
    &Spell::EffectDummy,                                    //  3 SPELL_EFFECT_DUMMY
    &Spell::EffectNULL,                                     //  4 SPELL_EFFECT_PORTAL_TELEPORT          unused
    &Spell::EffectTeleportUnits,                            //  5 SPELL_EFFECT_TELEPORT_UNITS
    &Spell::EffectApplyAura,                                //  6 SPELL_EFFECT_APPLY_AURA
    &Spell::EffectEnvirinmentalDMG,                         //  7 SPELL_EFFECT_ENVIRONMENTAL_DAMAGE
    &Spell::EffectManaDrain,                                //  8 SPELL_EFFECT_MANA_DRAIN
    &Spell::EffectHealthLeech,                              //  9 SPELL_EFFECT_HEALTH_LEECH
    &Spell::EffectHeal,                                     // 10 SPELL_EFFECT_HEAL
    &Spell::EffectNULL,                                     // 11 SPELL_EFFECT_BIND
    &Spell::EffectNULL,                                     // 12 SPELL_EFFECT_PORTAL
    &Spell::EffectNULL,                                     // 13 SPELL_EFFECT_RITUAL_BASE              unused
    &Spell::EffectNULL,                                     // 14 SPELL_EFFECT_RITUAL_SPECIALIZE        unused
    &Spell::EffectNULL,                                     // 15 SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL   unused
    &Spell::EffectQuestComplete,                            // 16 SPELL_EFFECT_QUEST_COMPLETE
    &Spell::EffectWeaponDmg,                                // 17 SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL
    &Spell::EffectResurrect,                                // 18 SPELL_EFFECT_RESURRECT
    &Spell::EffectAddExtraAttacks,                          // 19 SPELL_EFFECT_ADD_EXTRA_ATTACKS
    &Spell::EffectNULL,                                     // 20 SPELL_EFFECT_DODGE                    one spell: Dodge
    &Spell::EffectNULL,                                     // 21 SPELL_EFFECT_EVADE                    one spell: Evade (DND)
    &Spell::EffectParry,                                    // 22 SPELL_EFFECT_PARRY
    &Spell::EffectNULL,                                     // 23 SPELL_EFFECT_BLOCK                    one spell: Block
    &Spell::EffectCreateItem,                               // 24 SPELL_EFFECT_CREATE_ITEM
    &Spell::EffectNULL,                                     // 25 SPELL_EFFECT_WEAPON
    &Spell::EffectNULL,                                     // 26 SPELL_EFFECT_DEFENSE                  one spell: Defense
    &Spell::EffectPersistentAA,                             // 27 SPELL_EFFECT_PERSISTENT_AREA_AURA
    &Spell::EffectSummon,                                   // 28 SPELL_EFFECT_SUMMON
    &Spell::EffectMomentMove,                               // 29 SPELL_EFFECT_LEAP
    &Spell::EffectEnergize,                                 // 30 SPELL_EFFECT_ENERGIZE
    &Spell::EffectWeaponDmg,                                // 31 SPELL_EFFECT_WEAPON_PERCENT_DAMAGE
    &Spell::EffectNULL,                                     // 32 SPELL_EFFECT_TRIGGER_MISSILE
    &Spell::EffectOpenLock,                                 // 33 SPELL_EFFECT_OPEN_LOCK
    &Spell::EffectSummonChangeItem,                         // 34 SPELL_EFFECT_SUMMON_CHANGE_ITEM
    &Spell::EffectApplyAA,                                  // 35 SPELL_EFFECT_APPLY_AREA_AURA
    &Spell::EffectLearnSpell,                               // 36 SPELL_EFFECT_LEARN_SPELL
    &Spell::EffectNULL,                                     // 37 SPELL_EFFECT_SPELL_DEFENSE            one spell: SPELLDEFENSE (DND)
    &Spell::EffectDispel,                                   // 38 SPELL_EFFECT_DISPEL
    &Spell::EffectNULL,                                     // 39 SPELL_EFFECT_LANGUAGE
    &Spell::EffectDualWield,                                // 40 SPELL_EFFECT_DUAL_WIELD
    &Spell::EffectSummonWild,                               // 41 SPELL_EFFECT_SUMMON_WILD
    &Spell::EffectSummonGuardian,                           // 42 SPELL_EFFECT_SUMMON_GUARDIAN
    &Spell::EffectTeleUnitsFaceCaster,                      // 43 SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER
    &Spell::EffectLearnSkill,                               // 44 SPELL_EFFECT_SKILL_STEP
    &Spell::EffectAddHonor,                                 // 45 SPELL_EFFECT_ADD_HONOR                honor/pvp related
    &Spell::EffectNULL,                                     // 46 SPELL_EFFECT_SPAWN                    we must spawn pet there
    &Spell::EffectTradeSkill,                               // 47 SPELL_EFFECT_TRADE_SKILL
    &Spell::EffectNULL,                                     // 48 SPELL_EFFECT_STEALTH                  one spell: Base Stealth
    &Spell::EffectNULL,                                     // 49 SPELL_EFFECT_DETECT                   one spell: Detect
    &Spell::EffectTransmitted,                              // 50 SPELL_EFFECT_TRANS_DOOR
    &Spell::EffectNULL,                                     // 51 SPELL_EFFECT_FORCE_CRITICAL_HIT       unused
    &Spell::EffectNULL,                                     // 52 SPELL_EFFECT_GUARANTEE_HIT            one spell: zzOLDCritical Shot
    &Spell::EffectEnchantItemPerm,                          // 53 SPELL_EFFECT_ENCHANT_ITEM
    &Spell::EffectEnchantItemTmp,                           // 54 SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY
    &Spell::EffectTameCreature,                             // 55 SPELL_EFFECT_TAMECREATURE
    &Spell::EffectSummonPet,                                // 56 SPELL_EFFECT_SUMMON_PET
    &Spell::EffectLearnPetSpell,                            // 57 SPELL_EFFECT_LEARN_PET_SPELL
    &Spell::EffectWeaponDmg,                                // 58 SPELL_EFFECT_WEAPON_DAMAGE
    &Spell::EffectOpenSecretSafe,                           // 59 SPELL_EFFECT_OPEN_LOCK_ITEM
    &Spell::EffectProficiency,                              // 60 SPELL_EFFECT_PROFICIENCY
    &Spell::EffectSendEvent,                                // 61 SPELL_EFFECT_SEND_EVENT
    &Spell::EffectPowerBurn,                                // 62 SPELL_EFFECT_POWER_BURN
    &Spell::EffectThreat,                                   // 63 SPELL_EFFECT_THREAT
    &Spell::EffectTriggerSpell,                             // 64 SPELL_EFFECT_TRIGGER_SPELL
    &Spell::EffectNULL,                                     // 65 SPELL_EFFECT_HEALTH_FUNNEL            unused
    &Spell::EffectNULL,                                     // 66 SPELL_EFFECT_POWER_FUNNEL             unused
    &Spell::EffectHealMaxHealth,                            // 67 SPELL_EFFECT_HEAL_MAX_HEALTH
    &Spell::EffectInterruptCast,                            // 68 SPELL_EFFECT_INTERRUPT_CAST
    &Spell::EffectDistract,                                 // 69 SPELL_EFFECT_DISTRACT
    &Spell::EffectPull,                                     // 70 SPELL_EFFECT_PULL                     one spell: Distract Move
    &Spell::EffectPickPocket,                               // 71 SPELL_EFFECT_PICKPOCKET
    &Spell::EffectAddFarsight,                              // 72 SPELL_EFFECT_ADD_FARSIGHT
    &Spell::EffectSummonGuardian,                           // 73 SPELL_EFFECT_SUMMON_POSSESSED
    &Spell::EffectSummonTotem,                              // 74 SPELL_EFFECT_SUMMON_TOTEM
    &Spell::EffectHealMechanical,                           // 75 SPELL_EFFECT_HEAL_MECHANICAL          one spell: Mechanical Patch Kit
    &Spell::EffectSummonObjectWild,                         // 76 SPELL_EFFECT_SUMMON_OBJECT_WILD
    &Spell::EffectScriptEffect,                             // 77 SPELL_EFFECT_SCRIPT_EFFECT
    &Spell::EffectNULL,                                     // 78 SPELL_EFFECT_ATTACK
    &Spell::EffectSanctuary,                                // 79 SPELL_EFFECT_SANCTUARY
    &Spell::EffectAddComboPoints,                           // 80 SPELL_EFFECT_ADD_COMBO_POINTS
    &Spell::EffectNULL,                                     // 81 SPELL_EFFECT_CREATE_HOUSE             one spell: Create House (TEST)
    &Spell::EffectNULL,                                     // 82 SPELL_EFFECT_BIND_SIGHT
    &Spell::EffectDuel,                                     // 83 SPELL_EFFECT_DUEL
    &Spell::EffectStuck,                                    // 84 SPELL_EFFECT_STUCK
    &Spell::EffectSummonPlayer,                             // 85 SPELL_EFFECT_SUMMON_PLAYER
    &Spell::EffectNULL,                                     // 86 SPELL_EFFECT_ACTIVATE_OBJECT
    &Spell::EffectSummonTotem,                              // 87 SPELL_EFFECT_SUMMON_TOTEM_SLOT1
    &Spell::EffectSummonTotem,                              // 88 SPELL_EFFECT_SUMMON_TOTEM_SLOT2
    &Spell::EffectSummonTotem,                              // 89 SPELL_EFFECT_SUMMON_TOTEM_SLOT3
    &Spell::EffectSummonTotem,                              // 90 SPELL_EFFECT_SUMMON_TOTEM_SLOT4
    &Spell::EffectNULL,                                     // 91 SPELL_EFFECT_THREAT_ALL               one spell: zzOLDBrainwash
    &Spell::EffectEnchantHeldItem,                          // 92 SPELL_EFFECT_ENCHANT_HELD_ITEM
    &Spell::EffectNULL,                                     // 93 SPELL_EFFECT_SUMMON_PHANTASM
    &Spell::EffectSelfResurrect,                            // 94 SPELL_EFFECT_SELF_RESURRECT
    &Spell::EffectSkinning,                                 // 95 SPELL_EFFECT_SKINNING
    &Spell::EffectCharge,                                   // 96 SPELL_EFFECT_CHARGE
    &Spell::EffectSummonCritter,                            // 97 SPELL_EFFECT_SUMMON_CRITTER
    &Spell::EffectKnockBack,                                // 98 SPELL_EFFECT_KNOCK_BACK
    &Spell::EffectDisEnchant,                               // 99 SPELL_EFFECT_DISENCHANT
    &Spell::EffectInebriate,                                //100 SPELL_EFFECT_INEBRIATE
    &Spell::EffectFeedPet,                                  //101 SPELL_EFFECT_FEED_PET
    &Spell::EffectDismissPet,                               //102 SPELL_EFFECT_DISMISS_PET
    &Spell::EffectReputation,                               //103 SPELL_EFFECT_REPUTATION
    &Spell::EffectSummonObject,                             //104 SPELL_EFFECT_SUMMON_OBJECT_SLOT1
    &Spell::EffectSummonObject,                             //105 SPELL_EFFECT_SUMMON_OBJECT_SLOT2
    &Spell::EffectSummonObject,                             //106 SPELL_EFFECT_SUMMON_OBJECT_SLOT3
    &Spell::EffectSummonObject,                             //107 SPELL_EFFECT_SUMMON_OBJECT_SLOT4
    &Spell::EffectDispelMechanic,                           //108 SPELL_EFFECT_DISPEL_MECHANIC
    &Spell::EffectSummonDeadPet,                            //109 SPELL_EFFECT_SUMMON_DEAD_PET
    &Spell::EffectDestroyAllTotems,                         //110 SPELL_EFFECT_DESTROY_ALL_TOTEMS
    &Spell::EffectDurabilityDamage,                         //111 SPELL_EFFECT_DURABILITY_DAMAGE
    &Spell::EffectSummonDemon,                              //112 SPELL_EFFECT_SUMMON_DEMON
    &Spell::EffectResurrectNew,                             //113 SPELL_EFFECT_RESURRECT_NEW
    &Spell::EffectTaunt,                                    //114 SPELL_EFFECT_ATTACK_ME
    &Spell::EffectDurabilityDamagePCT,                      //115 SPELL_EFFECT_DURABILITY_DAMAGE_PCT
    &Spell::EffectSkinPlayerCorpse,                         //116 SPELL_EFFECT_SKIN_PLAYER_CORPSE       one spell: Remove Insignia, bg usage, required special corpse flags...
    &Spell::EffectSpiritHeal,                               //117 SPELL_EFFECT_SPIRIT_HEAL              one spell: Spirit Heal
    &Spell::EffectSkill,                                    //118 SPELL_EFFECT_SKILL                    professions and more
    &Spell::EffectApplyPetAura,                             //119 SPELL_EFFECT_APPLY_AURA_NEW
    &Spell::EffectNULL,                                     //120 SPELL_EFFECT_TELEPORT_GRAVEYARD       one spell: Graveyard Teleport Test
    &Spell::EffectWeaponDmg,                                //121 SPELL_EFFECT_NORMALIZED_WEAPON_DMG
    &Spell::EffectNULL,                                     //122 SPELL_EFFECT_122                      unused
    &Spell::EffectNULL,                                     //123 SPELL_EFFECT_SEND_TAXI                taxi/flight related (misc value is taxi path id)
    &Spell::EffectPlayerPull,                               //124 SPELL_EFFECT_PLAYER_PULL              opposite of knockback effect (pulls player twoard caster)
    &Spell::EffectReduceThreatPercent,                      //125 SPELL_EFFECT_REDUCE_THREAT_PERCENT
    &Spell::EffectStealBeneficialBuff,                      //126 SPELL_EFFECT_STEAL_BENEFICIAL_BUFF    spell steal effect?
    &Spell::EffectProspecting,                              //127 SPELL_EFFECT_PROSPECTING              Prospecting spell
    &Spell::EffectApplyAura,                                //128 SPELL_EFFECT_APPLY_AURA_NEW2          probably apply aura again
    &Spell::EffectNULL,                                     //129 SPELL_EFFECT_APPLY_AURA_NEW3
    &Spell::EffectNULL,                                     //130 SPELL_EFFECT_REDIRECT_THREAT
    &Spell::EffectNULL,                                     //131 SPELL_EFFECT_131                      used in some test spells
    &Spell::EffectNULL,                                     //132 SPELL_EFFECT_PLAY_MUSIC               sound id in misc value
    &Spell::EffectUnlearnSpecialization,                    //133 SPELL_EFFECT_UNLEARN_SPECIALIZATION   unlearn profession specialization
    &Spell::EffectNULL,                                     //134 SPELL_EFFECT_KILL_CREDIT              misc value is creature entry
    &Spell::EffectNULL,                                     //135 SPELL_EFFECT_CALL_PET
    &Spell::EffectNULL,                                     //136 SPELL_EFFECT_136
    &Spell::EffectNULL,                                     //137 SPELL_EFFECT_137
    &Spell::EffectNULL,                                     //138 SPELL_EFFECT_138
    &Spell::EffectNULL,                                     //139 SPELL_EFFECT_139                      unused
    &Spell::EffectNULL,                                     //140 SPELL_EFFECT_140
    &Spell::EffectNULL,                                     //141 SPELL_EFFECT_141
    &Spell::EffectTriggerSpellWithValue,                    //142 SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE
    &Spell::EffectNULL,                                     //143 SPELL_EFFECT_APPLY_AURA_NEW4
    &Spell::EffectNULL,                                     //144 SPELL_EFFECT_144                      Spectral Blast
    &Spell::EffectNULL,                                     //145 SPELL_EFFECT_145                      Black Hole Effect
    &Spell::EffectNULL,                                     //146 SPELL_EFFECT_146                      unused
    &Spell::EffectNULL,                                     //147 SPELL_EFFECT_147                      Torch Tossing Training Failure
    &Spell::EffectNULL,                                     //148 SPELL_EFFECT_148                      unused
    &Spell::EffectNULL,                                     //149 SPELL_EFFECT_149
    &Spell::EffectNULL,                                     //150 SPELL_EFFECT_150                      unused
    &Spell::EffectTriggerSpell,                             //151 SPELL_EFFECT_TRIGGER_SPELL_2
    &Spell::EffectNULL,                                     //152 SPELL_EFFECT_152                      RAFS
    &Spell::EffectNULL,                                     //153 SPELL_EFFECT_CREATE_PET               misc value is creature entry
};

void Spell::EffectNULL(uint32 /*i*/)
{
    sLog.outDebug("WORLD: Spell Effect DUMMY");
}

void Spell::EffectResurrectNew(uint32 i)
{
    if(!unitTarget) return;
    if(unitTarget->GetTypeId() != TYPEID_PLAYER) return;
    if(unitTarget->isAlive()) return;
    if(!unitTarget->IsInWorld()) return;

    uint32 health = m_currentBasePoints[i]+1;
    uint32 mana = m_spellInfo->EffectMiscValue[i];
    ((Player*)unitTarget)->setResurrect(m_caster->GetGUID(), m_caster->GetMapId(), m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), health, mana);
    SendResurrectRequest((Player*)unitTarget);
}

void Spell::EffectInstaKill(uint32 /*i*/)
{
    if( !unitTarget || !unitTarget->isAlive() )
        return;

    // Demonic Sacrifice
    if(m_spellInfo->Id==18788 && unitTarget->GetTypeId()==TYPEID_UNIT)
    {
        uint32 entry = unitTarget->GetEntry();
        uint32 spellID;
        switch(entry)
        {
            case   416: spellID=18789; break;               //imp
            case   417: spellID=18792; break;               //fellhunter
            case  1860: spellID=18790; break;               //void
            case  1863: spellID=18791; break;               //succubus
            case 17252: spellID=35701; break;               //fellguard
            default:
                sLog.outError("EffectInstaKill: Unhandled creature entry (%u) case.",entry);
                return;
        }

        m_caster->CastSpell(m_caster,spellID,true);
    }

    uint32 health = unitTarget->GetHealth();
    m_caster->DealDamage(unitTarget, health, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
}

void Spell::EffectEnvirinmentalDMG(uint32 i)
{
    uint32 absorb = 0;
    uint32 resist = 0;

    // Note: this hack with damage replace required until GO casting not implemented
    // environment damage spells already have around enemies targeting but this not help in case not existed GO casting support
    // currently each enemy selected explicitly and self cast damage, we prevent apply self casted spell bonuses/etc
    damage = m_spellInfo->EffectBasePoints[i]+m_spellInfo->EffectBaseDice[i];

    m_caster->CalcAbsorbResist(m_caster,GetSpellSchoolMask(m_spellInfo), SPELL_DIRECT_DAMAGE, damage, &absorb, &resist);

    m_caster->SendSpellNonMeleeDamageLog(m_caster, m_spellInfo->Id, damage, GetSpellSchoolMask(m_spellInfo), absorb, resist, false, 0, false);
    if(m_caster->GetTypeId() == TYPEID_PLAYER)
        ((Player*)m_caster)->EnvironmentalDamage(m_caster->GetGUID(),DAMAGE_FIRE,damage);
}

void Spell::EffectSchoolDMG(uint32 effect_idx)
{
    if( unitTarget && unitTarget->isAlive())
    {
        switch(m_spellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                //Gore
                if(m_spellInfo->SpellIconID == 2269 )
                {    
                    damage+= rand()%2 ? damage : 0;
                }

                // Meteor like spells (divided damage to targets)
                switch(m_spellInfo->Id)                     // better way to check unknown 
                {
                    case 24340: case 26558: case 28884: case 41276: 
                                                            // Meteor
                    case 26789:                             // Shard of the Fallen Star
                    case 31436:                             // Malevolent Cleave
                    case 35181:                             // Dive Bomb
                    case 40810: case 43267: case 43268:     // Saber Lash
                    case 42384:                             // Brutal Swipe
                    case 45150:                             // Meteor Slash
                    {
                        uint32 count = 0;
                        for(std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin();ihit != m_UniqueTargetInfo.end();++ihit)
                            if(ihit->effectMask & (1<<effect_idx))
                                ++count;

                        damage /= count;                    // divide to all targets
                        break;
                    }
                }

                break;
            }

            case SPELLFAMILY_MAGE:
            {
                // Arcane Blast
                if(m_spellInfo->SpellFamilyFlags & 0x20000000LL)
                {
                    m_caster->CastSpell(m_caster,36032,true);
                }
                // Blizzard
                else if(m_spellInfo->SpellFamilyFlags & 0x80LL)
                {
                    Aura* aura = NULL;
                    uint32 spell_id = 0;
                    Unit::AuraList const& mOverrideClassScript = m_caster->GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
                    for(Unit::AuraList::const_iterator i = mOverrideClassScript.begin(); i != mOverrideClassScript.end(); ++i)
                    {
                        switch((*i)->GetModifier()->m_miscvalue)
                        {
                            //Improved Blizzard
                            case 836: spell_id = 12484; aura = *i; break;
                            case 988: spell_id = 12485; aura = *i; break;
                            case 989: spell_id = 12486; aura = *i; break;
                        }
                        if(aura)
                            break;
                    }

                    if(aura)
                        unitTarget->CastSpell(unitTarget,spell_id,true,NULL,aura);
                }
                break;
            }
            case SPELLFAMILY_WARRIOR:
            {
                // Bloodthirst
                if(m_spellInfo->SpellFamilyFlags & 0x40000000000LL)
                {
                    damage = uint32(damage * (m_caster->GetTotalAttackPowerValue(BASE_ATTACK)) / 100);
                }
                // Shield Slam
                else if(m_spellInfo->SpellFamilyFlags & 0x100000000LL)
                    damage += int32(m_caster->GetShieldBlockValue());
                // Victory Rush
                else if(m_spellInfo->SpellFamilyFlags & 0x10000000000LL)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.45);
                    m_caster->ModifyAuraState(AURA_STATE_WARRIOR_VICTORY_RUSH, false);
                }
                break;
            }
            case SPELLFAMILY_WARLOCK:
            {
                // Incinerate Rank 1 & 2
                if((m_spellInfo->SpellFamilyFlags & 0x00004000000000LL) && m_spellInfo->SpellIconID==2128)
                {
                    // Incinerate does more dmg (dmg*0.25) if the target is Immolated.
                    if(unitTarget->HasAuraState(AURA_STATE_IMMOLATE))
                        damage += int32(damage*0.25);
                }
                break;
            }
            case SPELLFAMILY_DRUID:
            {
                // Ferocious Bite
                if(m_caster->GetTypeId()==TYPEID_PLAYER && (m_spellInfo->SpellFamilyFlags & 0x000800000) && m_spellInfo->SpellVisual==6587)
                {
                    // 15% for 5 combo
                    float percent = ((Player*)m_caster)->GetComboPoints() * 0.031f;
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * percent);
                    damage += int32(m_caster->GetPower(POWER_ENERGY) * m_spellInfo->DmgMultiplier[effect_idx]);
                    m_caster->SetPower(POWER_ENERGY,0);
                }
                // Swipe
                else if(m_spellInfo->SpellFamilyFlags & 0x0010000000000000LL)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.08f);
                }
                // Starfire
                else if ( m_spellInfo->SpellFamilyFlags & 0x0004LL )
                {
                    // Nordrassil Regalia - bonus
                    Unit::AuraList const& m_OverrideClassScript = m_caster->GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
                    for(Unit::AuraList::const_iterator i = m_OverrideClassScript.begin(); i != m_OverrideClassScript.end(); ++i)
                    {
                        // Starfire Bonus (caster)
                        if ( (*i)->GetModifier()->m_miscvalue == 5481 )
                        {
                            Unit::AuraList const& m_periodicDamageAuras = unitTarget->GetAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                            for(Unit::AuraList::const_iterator itr = m_periodicDamageAuras.begin(); itr != m_periodicDamageAuras.end(); ++itr)
                            {
                                // Moonfire or Insect Swarm (target debuff from any casters)
                                if ( (*itr)->GetSpellProto()->SpellFamilyFlags & 0x00200002LL )
                                {
                                    int32 mod = (*i)->GetModifier()->m_amount;
                                    damage += damage*mod/100;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
                break;
            }
            case SPELLFAMILY_ROGUE:
            {
                // Envenom
                if(m_caster->GetTypeId()==TYPEID_PLAYER && (m_spellInfo->SpellFamilyFlags & 0x800000000LL))
                {
                    // consume from stack dozes not more that have combo-points
                    if(uint32 combo = ((Player*)m_caster)->GetComboPoints())
                    {
                        // count consumed deadly poison doses at target
                        uint32 doses = 0;
                        uint32 extraDamage = 0;

                        // remove consumed poison doses
                        Unit::AuraList const& auras = unitTarget->GetAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                        for(Unit::AuraList::const_iterator itr = auras.begin(); itr!=auras.end() && combo;)
                        {
                            // Deadly poison (only attacker applied)
                            if( (*itr)->GetSpellProto()->SpellFamilyName==SPELLFAMILY_ROGUE && ((*itr)->GetSpellProto()->SpellFamilyFlags & 0x10000) && 
                                (*itr)->GetSpellProto()->SpellVisual==5100 && (*itr)->GetCasterGUID()==m_caster->GetGUID() )
                            {
                                --combo;
                                ++doses;

                                unitTarget->RemoveSingleAuraFromStack((*itr)->GetId(), (*itr)->GetEffIndex());

                                itr = auras.begin();
                            }
                            else
                                ++itr;
                        }

                        damage *= doses;
                        damage += int32(((Player*)m_caster)->GetTotalAttackPowerValue(BASE_ATTACK) * 0.03f * doses);

                        // Eviscerate and Envenom Bonus Damage (item set effect)
                        if(m_caster->GetDummyAura(37169))
                            damage += ((Player*)m_caster)->GetComboPoints()*40;
                    }
                }
                // Eviscerate
                else if((m_spellInfo->SpellFamilyFlags & 0x00020000LL) && m_caster->GetTypeId()==TYPEID_PLAYER)
                {
                    if(uint32 combo = ((Player*)m_caster)->GetComboPoints())
                    {
                        damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * combo * 0.03f);

                        // Eviscerate and Envenom Bonus Damage (item set effect)
                        if(m_caster->GetDummyAura(37169))
                            damage += combo*40;
                    }
                }
                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                // Mongoose Bite
                if((m_spellInfo->SpellFamilyFlags & 0x000000002) && m_spellInfo->SpellVisual==342)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.2);
                }
                // Arcane Shot
                else if((m_spellInfo->SpellFamilyFlags & 0x00000800) && m_spellInfo->maxLevel > 0)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(RANGED_ATTACK)*0.15);
                }
                // Steady Shot
                else if(m_spellInfo->SpellFamilyFlags & 0x100000000LL)
                {
                    int32 base = irand((int32)m_caster->GetWeaponDamageRange(RANGED_ATTACK, MINDAMAGE),(int32)m_caster->GetWeaponDamageRange(RANGED_ATTACK, MAXDAMAGE));
                    damage += int32(float(base)/m_caster->GetAttackTime(RANGED_ATTACK)*2800 + m_caster->GetTotalAttackPowerValue(RANGED_ATTACK)*0.2f);
                }
                //Explosive Trap Effect
                else if(m_spellInfo->SpellFamilyFlags & 0x00000004)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(RANGED_ATTACK)*0.1);
                }
                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                //Judgement of Vengeance
                if((m_spellInfo->SpellFamilyFlags & 0x800000000LL) && m_spellInfo->SpellIconID==2292)
                {
                    uint32 stacks = 0;
                    Unit::AuraList const& auras = unitTarget->GetAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                    for(Unit::AuraList::const_iterator itr = auras.begin(); itr!=auras.end(); ++itr)
                        if((*itr)->GetId() == 31803 && (*itr)->GetCasterGUID()==m_caster->GetGUID())
                            ++stacks;
                    if(!stacks)
                        //No damage if the target isn't affected by this
                        damage = -1;
                    else
                        damage *= stacks;
                }
                break;
            }
        }

        if(damage >= 0)
        {
            uint32 finalDamage;
            if(m_originalCaster)                            // m_caster only passive source of cast
                finalDamage = m_originalCaster->SpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, damage, m_IsTriggeredSpell, true);
            else
                finalDamage = m_caster->SpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, damage, m_IsTriggeredSpell, true);

            // post effects
            switch(m_spellInfo->SpellFamilyName)
            {
                case SPELLFAMILY_WARRIOR:
                {
                    // Bloodthirst
                    if(m_spellInfo->SpellFamilyFlags & 0x40000000000LL)
                    {
                        uint32 BTAura = 0;
                        switch(m_spellInfo->Id)
                        {
                            case 23881: BTAura = 23885; break;
                            case 23892: BTAura = 23886; break;
                            case 23893: BTAura = 23887; break;
                            case 23894: BTAura = 23888; break;
                            case 25251: BTAura = 25252; break;
                            case 30335: BTAura = 30339; break;
                            default:
                                sLog.outError("Spell::EffectSchoolDMG: Spell %u not handled in BTAura",m_spellInfo->Id);
                                break;
                        }

                        if (BTAura)
                            m_caster->CastSpell(m_caster,BTAura,true);
                    }
                    break;
                }
                case SPELLFAMILY_PRIEST:
                {
                    // Shadow Word: Death
                    if(finalDamage > 0 && (m_spellInfo->SpellFamilyFlags & 0x0000000200000000LL) && unitTarget->isAlive())
                        // deals damage equal to damage done to caster if victim is not killed
                        m_caster->SpellNonMeleeDamageLog( m_caster, m_spellInfo->Id, finalDamage, m_IsTriggeredSpell, false);

                    break;
                }
                case SPELLFAMILY_PALADIN:
                {
                    // Judgement of Blood
                    if(finalDamage > 0 && (m_spellInfo->SpellFamilyFlags & 0x0000000800000000LL) && m_spellInfo->SpellIconID==153)
                    {
                        int32 damagePoint  = finalDamage * 33 / 100;
                        m_caster->CastCustomSpell(m_caster, 32220, &damagePoint, NULL, NULL, true);
                    }
                    break;
                }
            }
        }
    }

    if (m_caster->GetTypeId()==TYPEID_PLAYER && m_spellInfo->Attributes == 0x150010)
        m_caster->AttackStop();
}

void Spell::EffectDummy(uint32 i)
{
    if(!unitTarget && !gameObjTarget && !itemTarget)
        return;

    // selection by spell family
    switch(m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
            // Gnomish Poultryizer trinket
            switch(m_spellInfo->Id )
            {
                case 8063:                                  // Deviate Fish
                {
                    if(m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    uint32 spell_id = 0;
                    switch(urand(1,5))
                    {
                        case 1: spell_id = 8064; break;     // Sleepy
                        case 2: spell_id = 8065; break;     // Invigorate
                        case 3: spell_id = 8066; break;     // Shrink
                        case 4: spell_id = 8067; break;     // Party Time!
                        case 5: spell_id = 8068; break;     // Healthy Spirit
                    }
                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 8213:                                  // Savory Deviate Delight
                {
                    if(m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    uint32 spell_id = 0;
                    switch(urand(1,2))
                    {
                        // Flip Out - ninja
                        case 1: spell_id = (m_caster->getGender() == GENDER_MALE ? 8219 : 8220); break;
                        // Yaaarrrr - pirate
                        case 2: spell_id = (m_caster->getGender() == GENDER_MALE ? 8221 : 8222); break;
                    }
                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 8593:                                  // Symbol of life (restore creature to life)
                case 31225:                                 // Shimmering Vessel (restore creature to life)
                {
                    if(!unitTarget || unitTarget->GetTypeId()!=TYPEID_UNIT)
                        return;
                    ((Creature*)unitTarget)->setDeathState(JUST_ALIVED);
                    return;
                }
                case 12162:                                 // Deep wounds
                case 12850:                                 // (now good common check for this spells)
                case 12868:
                {
                    if(!unitTarget)
                        return;

                    float damage;
                    // DW should benefit of attack power, damage percent mods etc.
                    // TODO: check if using offhand damage is correct and if it should be divided by 2
                    if (m_caster->haveOffhandWeapon() && m_caster->getAttackTimer(BASE_ATTACK) > m_caster->getAttackTimer(OFF_ATTACK))
                        damage = (m_caster->GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE) + m_caster->GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE))/2;
                    else
                        damage = (m_caster->GetFloatValue(UNIT_FIELD_MINDAMAGE) + m_caster->GetFloatValue(UNIT_FIELD_MAXDAMAGE))/2;

                    switch (m_spellInfo->Id)
                    {
                        case 12850: damage *= 0.2f; break;
                        case 12162: damage *= 0.4f; break;
                        case 12868: damage *= 0.6f; break;
                        default:
                            sLog.outError("Spell::EffectDummy: Spell %u not handled in DW",m_spellInfo->Id);
                            return;
                    };

                    int32 deepWoundsDotBasePoints0 = int32(damage / 4);
                    m_caster->CastCustomSpell(unitTarget, 12721, &deepWoundsDotBasePoints0, NULL, NULL, true, NULL);
                    return;
                }
                case 12975:                                 //Last Stand
                {
                    int32 healthModSpellBasePoints0 = int32(m_caster->GetMaxHealth()*0.3);
                    m_caster->CastCustomSpell(m_caster, 12976, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
                    return;
                }
                case 13120:                                 // net-o-matic
                {
                    if(!unitTarget)
                        return;

                    uint32 spell_id = 0;

                    uint32 roll = urand(0, 99);

                    if(roll < 2)                            // 2% for 30 sec self root (off-like chance unknown)
                        spell_id = 16566;
                    else if(roll < 4)                       // 2% for 20 sec root, charge to target (off-like chance unknown)
                        spell_id = 13119;
                    else                                    // normal root
                        spell_id = 13099;

                    m_caster->CastSpell(unitTarget,spell_id,true,NULL);
                    return;
                }
                case 14185:                                         //Preparation Rogue
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    //immediately finishes the cooldown on certain Rogue abilities
                    const PlayerSpellMap& sp_list = ((Player *)m_caster)->GetSpellMap();
                    for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
                    {
                        uint32 classspell = itr->first;
                        SpellEntry const *spellInfo = sSpellStore.LookupEntry(classspell);

                        if (spellInfo->SpellFamilyName == SPELLFAMILY_ROGUE && (spellInfo->SpellFamilyFlags & 0x26000000860LL))
                        {
                            ((Player*)m_caster)->RemoveSpellCooldown(classspell);

                            WorldPacket data(SMSG_CLEAR_COOLDOWN, (4+8));
                            data << uint32(classspell);
                            data << uint64(m_caster->GetGUID());
                            ((Player*)m_caster)->GetSession()->SendPacket(&data);
                        }
                    }
                    return;
                }
                case 16589:                                 // Noggenfogger Elixir
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    uint32 spell_id = 0;
                    switch(urand(1,3))
                    {
                        case 1: spell_id = 16595; break;
                        case 2: spell_id = 16593; break;
                        default:spell_id = 16591; break;
                    }

                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 17251:                                 // Spirit Healer Res
                {
                    if(!unitTarget || !m_originalCaster)
                        return;

                    if(m_originalCaster->GetTypeId() == TYPEID_PLAYER)
                    {
                        WorldPacket data(SMSG_SPIRIT_HEALER_CONFIRM, 8);
                        data << unitTarget->GetGUID();
                        ((Player*)m_originalCaster)->GetSession()->SendPacket( &data );
                    }
                    return;
                }
                case 17271:                                 // Test Fetid Skull
                {
                    if(!itemTarget && m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    uint32 spell_id = roll_chance_i(50) ? 17269 : 17270;

                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 20577:                                 // Cannibalize
                {
                    if (!unitTarget)
                        return;

                    // casting
                    m_caster->CastSpell(m_caster,20578,false,NULL);
                    return;
                }

                // Different item engineering summons
                case 23074:                                 // Arc. Dragonling
                    if (!m_CastItem) return;
                    m_caster->CastSpell(m_caster,19804,true,m_CastItem);
                    return;
                case 23075:                                 // Mithril Mechanical Dragonling
                    if (!m_CastItem) return;
                    m_caster->CastSpell(m_caster,12749,true,m_CastItem);
                    return;
                case 23076:                                 // Mechanical Dragonling
                    if (!m_CastItem) return;
                    m_caster->CastSpell(m_caster,4073,true,m_CastItem);
                    return;
                case 23019:                                 // Crystal Prison Dummy DND
                {
                    if(!unitTarget || !unitTarget->isAlive() || unitTarget->GetTypeId() != TYPEID_UNIT || ((Creature*)unitTarget)->isPet())
                        return;

                    Creature* creatureTarget = (Creature*)unitTarget;
                    if(creatureTarget->isPet())
                        return;

                    creatureTarget->setDeathState(JUST_DIED);
                    creatureTarget->RemoveCorpse();
                    creatureTarget->SetHealth(0);                   // just for nice GM-mode view

                    GameObject* pGameObj = new GameObject(m_caster);

                    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), 179644 ,creatureTarget->GetMapId(),
                        creatureTarget->GetPositionX(), creatureTarget->GetPositionY(), creatureTarget->GetPositionZ(),
                        creatureTarget->GetOrientation(), 0, 0, 0, 0, 100, 1) )
                    {
                        delete pGameObj;
                        return;
                    }

                    pGameObj->SetRespawnTime(creatureTarget->GetRespawnTime()-time(NULL));
                    pGameObj->SetOwnerGUID(m_caster->GetGUID() );
                    pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, m_caster->getLevel() );
                    pGameObj->SetSpellId(m_spellInfo->Id);

                    DEBUG_LOG("AddObject at SpellEfects.cpp EffectDummy\n");
                    MapManager::Instance().GetMap(creatureTarget->GetMapId(), pGameObj)->Add(pGameObj);

                    WorldPacket data(SMSG_GAMEOBJECT_SPAWN_ANIM_OBSOLETE, 8);
                    data << uint64(pGameObj->GetGUID());
                    m_caster->SendMessageToSet(&data,true);

                    return;
                }
                case 23133:                                 // Gnomish Battle Chicken
                    if (!m_CastItem) return;
                    m_caster->CastSpell(m_caster,13166,true,m_CastItem);
                    return;
                case 23448:                                 // Ultrasafe Transporter: Gadgetzan - backfires
                {
                    int32 r = irand(0, 119);
                    if ( r < 20 )                           // 1/6 polymorph
                        m_caster->CastSpell(m_caster,23444,true);
                    else if ( r < 100 )                     // 4/6 evil twin
                        m_caster->CastSpell(m_caster,23445,true);
                    else                                    // 1/6 miss the target
                        m_caster->CastSpell(m_caster,36902,true);
                    return;
                }
                case 23453:                                 // Ultrasafe Transporter: Gadgetzan
                {
                    if ( roll_chance_i(50) )                // success
                        m_caster->CastSpell(m_caster,23441,true);
                    else                                    // failure
                        m_caster->CastSpell(m_caster,23446,true);
                    return;
                }
                case 23725:                                 // Gift of Life (warrior bwl trinket)
                    m_caster->CastSpell(m_caster,23782,true);
                    m_caster->CastSpell(m_caster,23783,true);
                    return;
                case 25860:                                 // Reindeer Transformation
                {
                    if (!m_caster->HasAuraType(SPELL_AURA_MOUNTED))
                        return;

                    float flyspeed = m_caster->GetSpeedRate(MOVE_FLY);
                    float speed = m_caster->GetSpeedRate(MOVE_RUN);

                    m_caster->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);

                    //5 different spells used depending on mounted speed and if mount can fly or not
                    if (flyspeed >= 4.1f)
                        m_caster->CastSpell(m_caster, 44827, true); //310% flying Reindeer
                    else if (flyspeed >= 3.8f)
                        m_caster->CastSpell(m_caster, 44825, true); //280% flying Reindeer
                    else if (flyspeed >= 1.6f)
                        m_caster->CastSpell(m_caster, 44824, true); //60% flying Reindeer
                    else if (speed >= 2.0f)
                        m_caster->CastSpell(m_caster, 25859, true); //100% ground Reindeer
                    else
                        m_caster->CastSpell(m_caster, 25858, true); //60% ground Reindeer

                    return;
                }
                case 29200:                                 // Purify Helboar Meat
                {
                    if(!itemTarget && m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    uint32 spell_id = roll_chance_i(50) ? 29277 : 29278;

                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 30507:                                 // Poultryizer
                    if (!m_CastItem) return;
                    if(roll_chance_i(80))                   // success
                        m_caster->CastSpell(unitTarget, 30501, true, m_CastItem);
                    else                                    // backfire 20%
                        m_caster->CastSpell(unitTarget, 30504, true, m_CastItem);
                    return;
                case 30458:                                 // Nigh Invulnerability
                    if (!m_CastItem) return;
                    if(roll_chance_i(86))                   // success
                        m_caster->CastSpell(m_caster, 30456, true, m_CastItem);
                    else                                    // backfire in 14% casts
                        m_caster->CastSpell(m_caster, 30457, true, m_CastItem);
                    return;
                case 33060:                                         // Make a Wish
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    uint32 spell_id = 0;

                    switch(urand(1,5))
                    {
                        case 1: spell_id = 33053; break;
                        case 2: spell_id = 33057; break;
                        case 3: spell_id = 33059; break;
                        case 4: spell_id = 33062; break;
                        case 5: spell_id = 33064; break;
                    }

                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 29858:                                 // Soulshatter
                {
                    if (unitTarget && unitTarget->GetTypeId() == TYPEID_UNIT && unitTarget->IsHostileTo(m_caster))
                        m_caster->CastSpell(unitTarget,32835,true);
                    return;
                }
            }

            //All IconID Check in there
            switch(m_spellInfo->SpellIconID)
            {
                // Berserking (troll racial traits)
                case 1661:
                {
                    uint32 healthPerc = uint32((float(m_caster->GetHealth())/m_caster->GetMaxHealth())*100);
                    int32 melee_mod = 10;
                    if (healthPerc <= 40)
                        melee_mod = 30;
                    if (healthPerc < 100 && healthPerc > 40)
                        melee_mod = 10+(100-healthPerc)/3;

                    int32 hasteModBasePoints0 = melee_mod;          // (EffectBasePoints[0]+1)-1+(5-melee_mod) = (melee_mod-1+1)-1+5-melee_mod = 5-1
                    int32 hasteModBasePoints1 = (5-melee_mod);
                    int32 hasteModBasePoints2 = 5;
                    m_caster->CastCustomSpell(m_caster,26635,&hasteModBasePoints0,&hasteModBasePoints1,&hasteModBasePoints2,true,NULL);
                    return;
                }
            }
            break;
        case SPELLFAMILY_MAGE:
            switch(m_spellInfo->Id )
            {
                case 11958:                                 // Cold Snap
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    // immediately finishes the cooldown on Frost spells
                    const PlayerSpellMap& sp_list = ((Player *)m_caster)->GetSpellMap();
                    for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
                    {
                        if (itr->second->state == PLAYERSPELL_REMOVED)
                            continue;

                        uint32 classspell = itr->first;
                        SpellEntry const *spellInfo = sSpellStore.LookupEntry(classspell);

                        if( spellInfo->SpellFamilyName == SPELLFAMILY_MAGE && 
                            (GetSpellSchoolMask(spellInfo) & SPELL_SCHOOL_MASK_FROST) &&
                            spellInfo->Id != 11958 && GetSpellRecoveryTime(spellInfo) > 0 )
                        {
                            ((Player*)m_caster)->RemoveSpellCooldown(classspell);

                            WorldPacket data(SMSG_CLEAR_COOLDOWN, (4+8));
                            data << uint32(classspell);
                            data << uint64(m_caster->GetGUID());
                            ((Player*)m_caster)->GetSession()->SendPacket(&data);
                        }
                    }
                    return;
                }
            }
            break;
        case SPELLFAMILY_WARRIOR:
            // Charge
            if(m_spellInfo->SpellFamilyFlags & 0x1 && m_spellInfo->SpellVisual == 867)
            {
                int32 chargeBasePoints0 = damage;
                m_caster->CastCustomSpell(m_caster,34846,&chargeBasePoints0,NULL,NULL,true);
                return;
            }
            // Execute
            if(m_spellInfo->SpellFamilyFlags & 0x20000000)
            {
                if(!unitTarget)
                    return;

                int32 basePoints0 = damage+int32(m_caster->GetPower(POWER_RAGE) * m_spellInfo->DmgMultiplier[i]);
                m_caster->CastCustomSpell(unitTarget, 20647, &basePoints0, NULL, NULL, true, 0);
                m_caster->SetPower(POWER_RAGE,0);
                return;
            }
            if(m_spellInfo->Id==21977)                      //Warrior's Wrath
            {
                if(!unitTarget)
                    return;

                m_caster->CastSpell(unitTarget,21887,true); // spell mod
                return;
            }
            break;
        case SPELLFAMILY_WARLOCK:
            //Life Tap (only it have this with dummy effect)
            if (m_spellInfo->SpellFamilyFlags == 0x40000)
            {
                float cost = m_currentBasePoints[0]+1;

                if(Player* modOwner = m_caster->GetSpellModOwner())
                    modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_COST, cost,this);

                uint32 dmg = m_caster->SpellDamageBonus(m_caster, m_spellInfo,uint32(cost > 0 ? cost : 0), SPELL_DIRECT_DAMAGE);

                if(int32(m_caster->GetHealth()) > dmg)
                {
                    m_caster->SendSpellNonMeleeDamageLog(m_caster, m_spellInfo->Id, dmg, GetSpellSchoolMask(m_spellInfo), 0, 0, false, 0, false);
                    m_caster->DealDamage(m_caster,dmg,NULL,DIRECT_DAMAGE,GetSpellSchoolMask(m_spellInfo),m_spellInfo,false);

                    int32 mana = dmg;

                    Unit::AuraList const& auraDummy = m_caster->GetAurasByType(SPELL_AURA_DUMMY);
                    for(Unit::AuraList::const_iterator itr = auraDummy.begin(); itr != auraDummy.end(); ++itr)
                    {
                        // only Imp. Life Tap have this in combination with dummy aura
                        if((*itr)->GetSpellProto()->SpellFamilyName==SPELLFAMILY_WARLOCK && (*itr)->GetSpellProto()->SpellIconID == 208)
                            mana = ((*itr)->GetModifier()->m_amount + 100)* mana / 100;
                    }

                    m_caster->ModifyPower(POWER_MANA,mana);
                    m_caster->SendEnergizeSpellLog(m_caster, m_spellInfo->Id, mana, POWER_MANA,false);
                }
                else
                    SendCastResult(SPELL_FAILED_FIZZLE);
                return;
            }
            break;
        case SPELLFAMILY_PRIEST:
            switch(m_spellInfo->Id )
            {
                case 28598:                                 // Touch of Weakness triggered spell
                {
                    if(!unitTarget || !m_triggeredByAuraSpell)
                        return;

                    uint32 spellid = 0;
                    switch(m_triggeredByAuraSpell->Id)
                    {
                        case 2652:  spellid =  2943; break; // Rank 1
                        case 19261: spellid = 19249; break; // Rank 2
                        case 19262: spellid = 19251; break; // Rank 3
                        case 19264: spellid = 19252; break; // Rank 4
                        case 19265: spellid = 19253; break; // Rank 5
                        case 19266: spellid = 19254; break; // Rank 6
                        case 25461: spellid = 25460; break; // Rank 7
                        default:
                            sLog.outError("Spell::EffectDummy: Spell 28598 triggered by unhandeled spell %u",m_triggeredByAuraSpell->Id);
                            return;
                    }
                    m_caster->CastSpell(unitTarget, spellid, true, NULL);
                    return;
                }
            }
            break;
        case SPELLFAMILY_DRUID:
            switch(m_spellInfo->Id )
            {
                case 5420:                                  // Tree of Life passive
                {
                    // Tree of Life area effect
                    int32 health_mod = int32(m_caster->GetStat(STAT_SPIRIT)/4);
                    m_caster->CastCustomSpell(m_caster,34123,&health_mod,NULL,NULL,true,NULL);
                    return;
                }
            }
            break;
        case SPELLFAMILY_ROGUE:
            switch(m_spellInfo->Id )
            {
                case 31231:                                 // Cheat Death
                {
                    m_caster->CastSpell(m_caster,45182,true);
                    return;
                }
                case 5938:                                  // Shiv
                {
                    if(m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    Player *pCaster = ((Player*)m_caster);

                    Item *item = pCaster->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                    if(!item)
                        return;

                    // all poison enchantments is temporary
                    uint32 enchant_id = item->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT);
                    if(!enchant_id)
                        return;

                    SpellItemEnchantmentEntry const *pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
                    if(!pEnchant)
                        return;

                    for (int s=0;s<3;s++)
                    {
                        if(pEnchant->type[s]!=ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                            continue;

                        SpellEntry const* combatEntry = sSpellStore.LookupEntry(pEnchant->spellid[s]);
                        if(!combatEntry || combatEntry->Dispel != DISPEL_POISON)
                            continue;

                        m_caster->CastSpell(unitTarget, combatEntry, true, item);
                    }

                    m_caster->CastSpell(unitTarget, 5940, true);
                    return;
                }
            }
            break;
        case SPELLFAMILY_HUNTER:
            // Steady Shot
            if(m_spellInfo->SpellFamilyFlags & 0x100000000LL)
            {
                if( !unitTarget || !unitTarget->isAlive())
                    return;

                bool found = false;

                // check dazed affect
                Unit::AuraList const& decSpeedList = unitTarget->GetAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);
                for(Unit::AuraList::const_iterator iter = decSpeedList.begin(); iter != decSpeedList.end(); ++iter)
                {
                    if((*iter)->GetSpellProto()->SpellIconID==15 && (*iter)->GetSpellProto()->Dispel==0)
                    {
                        found = true;
                        break;
                    }
                }

                if(found)
                    m_caster->SpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, damage, m_IsTriggeredSpell, true);
                return;
            }
            // Kill command
            if(m_spellInfo->SpellFamilyFlags & 0x00080000000000LL)
            {
                if(m_caster->getClass()!=CLASS_HUNTER)
                    return;

                // clear hunter crit aura state
                m_caster->ModifyAuraState(AURA_STATE_HUNTER_CRIT_STRIKE,false);

                // additional damage from pet to pet target
                Pet* pet = m_caster->GetPet();
                if(!pet || !pet->getVictim())
                    return;

                uint32 spell_id = 0;
                switch (m_spellInfo->Id)
                {
                case 34026: spell_id = 34027; break;        // rank 1
                default:
                    sLog.outError("Spell::EffectDummy: Spell %u not handled in KC",m_spellInfo->Id);
                    return;
                }

                pet->CastSpell(pet->getVictim(), spell_id, true);
                return;
            }
            //Focused Fire (flags have more spells but only this have dummy effect)
            if (m_spellInfo->SpellIconID == 2221)
            {
                if(!unitTarget)
                    return;

                uint32 spell_id;

                switch(m_spellInfo->Id)
                {
                    case 35029: spell_id = 35060; break;    //Rank 1
                    case 35030: spell_id = 35061; break;    //Rank 2
                    default:
                        sLog.outError("Spell::EffectDummy: Spell %u not handled in FF",m_spellInfo->Id);
                        return;
                }

                /* FIX ME: must be applied as pet passive spell for _each_ pet after this talent learning
                Pet* pet = unitTarget->GetPet();
                if(!pet)
                    return;

                m_caster->CastSpell(pet, spell_id, true, 0);
                */
                return;
            }

            switch(m_spellInfo->Id)
            {
                case 23989:                                 //Readiness talent
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    //immediately finishes the cooldown for hunter abilities
                    const PlayerSpellMap& sp_list = ((Player *)m_caster)->GetSpellMap();
                    for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
                    {
                        uint32 classspell = itr->first;
                        SpellEntry const *spellInfo = sSpellStore.LookupEntry(classspell);

                        if (spellInfo->SpellFamilyName == SPELLFAMILY_HUNTER && spellInfo->Id != 23989 && GetSpellRecoveryTime(spellInfo) > 0 )
                        {
                            ((Player*)m_caster)->RemoveSpellCooldown(classspell);

                            WorldPacket data(SMSG_CLEAR_COOLDOWN, (4+8));
                            data << uint32(classspell);
                            data << uint64(m_caster->GetGUID());
                            ((Player*)m_caster)->GetSession()->SendPacket(&data);
                        }
                    }
                    return;
                }
                case 37506:                                 // Scatter Shot
                {
                    if (m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    // break Auto Shot
                    m_caster->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
                    return;
                }
            }
            break;
        case SPELLFAMILY_PALADIN:
            switch(m_spellInfo->SpellIconID)
            {
                case  156:                                  // Holy Shock
                {
                    if(!unitTarget)
                        return;

                    int hurt = 0;
                    int heal = 0;

                    switch(m_spellInfo->Id)
                    {
                        case 20473: hurt = 25912; heal = 25914; break;
                        case 20929: hurt = 25911; heal = 25913; break;
                        case 20930: hurt = 25902; heal = 25903; break;
                        case 27174: hurt = 27176; heal = 27175; break;
                        case 33072: hurt = 33073; heal = 33074; break;
                        default:
                            sLog.outError("Spell::EffectDummy: Spell %u not handled in HS",m_spellInfo->Id);
                            return;
                    }

                    if(m_caster->IsFriendlyTo(unitTarget))
                        m_caster->CastSpell(unitTarget, heal, true, 0);
                    else
                        m_caster->CastSpell(unitTarget, hurt, true, 0);

                    return;
                }
                case 561:                                   // Judgement of command
                {
                    if(!unitTarget)
                        return;

                    uint32 spell_id = m_currentBasePoints[i]+1;
                    SpellEntry const* spell_proto = sSpellStore.LookupEntry(spell_id);
                    if(!spell_proto)
                        return;

                    if( !unitTarget->hasUnitState(UNIT_STAT_STUNDED) && m_caster->GetTypeId()==TYPEID_PLAYER)
                    {
                        // decreased damage (/2) for non-stunned target.
                        SpellModifier *mod = new SpellModifier;
                        mod->op = SPELLMOD_DAMAGE;
                        mod->value = -50;
                        mod->type = SPELLMOD_PCT;
                        mod->spellId = m_spellInfo->Id;
                        mod->effectId = i;
                        mod->lastAffected = NULL;
                        mod->mask = 0x0000020000000000LL;
                        mod->charges = 0;

                        ((Player*)m_caster)->AddSpellMod(mod, true);
                        m_caster->CastSpell(unitTarget,spell_proto,true,NULL);
                                                            // mod deleted
                        ((Player*)m_caster)->AddSpellMod(mod, false);
                    }
                    else
                        m_caster->CastSpell(unitTarget,spell_proto,true,NULL);

                    return;
                }
            }

            switch(m_spellInfo->Id)
            {
                case 31789:                                 // Righteous Defense (step 1)
                {
                    // 31989 -> dummy effect (step 1) + dummy effect (step 2) -> 31709 (taunt like spell for each target)

                    // non-standard cast requirement check
                    if (!unitTarget || !unitTarget->hasUnitState(UNIT_STAT_ATTACK_BY))
                    {
                        // clear cooldown at fail
                        if(m_caster->GetTypeId()==TYPEID_PLAYER)
                        {
                            ((Player*)m_caster)->RemoveSpellCooldown(31989);

                            WorldPacket data(SMSG_CLEAR_COOLDOWN, (4+8));
                            data << uint32(31789);                  // spell id
                            data << uint64(m_caster->GetGUID());
                            ((Player*)m_caster)->GetSession()->SendPacket(&data);
                        }

                        SendCastResult(SPELL_FAILED_TARGET_AFFECTING_COMBAT);
                        return;
                    }

                    // Righteous Defense (step 2) (in old version 31980 dummy effect)
                    // Clear targets for eff 1
                    for(std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin();ihit != m_UniqueTargetInfo.end();++ihit)
                        if( ihit->effectMask & 0x01 )
                            ihit->effectMask = 0;

                    Unit::AttackerSet const& attackers = unitTarget->getAttackers();

                    // not attacker or list empty
                    if(attackers.empty())
                        return;

                    // chance to be selected from list
                    float chance = 100.0f/(attackers.size());
                    uint32 count=0;
                    for(Unit::AttackerSet::const_iterator aItr = attackers.begin(); aItr != attackers.end() && count < 3; ++aItr)
                    {
                        if(!roll_chance_f(chance))
                            continue;
                        ++count;
                        AddUnitTarget((*aItr), 1);
                    }

                    // now let next effect cast spell at each target.
                    return;
                }
                case 37877:                                 // Blessing of Faith
                {
                    if(!unitTarget)
                        return;

                    uint32 spell_id = 0;
                    switch(unitTarget->getClass())
                    {
                        case CLASS_DRUID:   spell_id = 37878; break;
                        case CLASS_PALADIN: spell_id = 37879; break;
                        case CLASS_PRIEST:  spell_id = 37880; break;
                        case CLASS_SHAMAN:  spell_id = 37881; break;
                        default: return;                    // ignore for not healing classes
                    }

                    m_caster->CastSpell(m_caster,spell_id,true);
                    return;
                }
            }
            break;
        case SPELLFAMILY_SHAMAN:
            //Shaman Rockbiter Weapon
            if (m_spellInfo->SpellFamilyFlags == 0x400000)
            {
                uint32 spell_id = 0;
                switch(m_spellInfo->Id)
                {
                    case  8017: spell_id = 36494; break;    // Rank 1
                    case  8018: spell_id = 36750; break;    // Rank 2
                    case  8019: spell_id = 36755; break;    // Rank 3
                    case 10399: spell_id = 36759; break;    // Rank 4
                    case 16314: spell_id = 36763; break;    // Rank 5
                    case 16315: spell_id = 36766; break;    // Rank 6
                    case 16316: spell_id = 36771; break;    // Rank 7
                    case 25479: spell_id = 36775; break;    // Rank 8
                    case 25485: spell_id = 36499; break;    // Rank 9
                    default:
                        sLog.outError("Spell::EffectDummy: Spell %u not handled in RW",m_spellInfo->Id);
                        return;
                }

                SpellEntry const *spellInfo = sSpellStore.LookupEntry( spell_id );

                if(!spellInfo)
                {
                    sLog.outError("WORLD: unknown spell id %i\n", spell_id);
                    return;
                }

                if(m_caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                if(Item* item = ((Player*)m_caster)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                {
                    if(item->IsFitToSpellRequirements(m_spellInfo))
                    {
                        Spell *spell = new Spell(m_caster, spellInfo, true);

                        // enchanting spell selected by calculated damage-per-sec in enchanting effect
                        // at calculation applied affect from Elemental Weapons talent
                        // real enchantment damage-1
                        spell->m_currentBasePoints[1] = damage-1;

                        SpellCastTargets targets;
                        targets.setItemTarget( item );
                        spell->prepare(&targets);
                    }
                }

                if(Item* item = ((Player*)m_caster)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                {
                    if(item->IsFitToSpellRequirements(m_spellInfo))
                    {
                        Spell *spell = new Spell(m_caster, spellInfo, true);

                        // enchanting spell selected by calculated damage-per-sec in enchanting effect
                        // at calculation applied affect from Elemental Weapons talent
                        // real enchantment damage-1
                        spell->m_currentBasePoints[1] = damage-1;

                        SpellCastTargets targets;
                        targets.setItemTarget( item );
                        spell->prepare(&targets);
                    }
                }

                return;
            }

            if(m_spellInfo->Id == 39610)                    // Mana-Tide Totem effect
            {
                if(!unitTarget || unitTarget->getPowerType() != POWER_MANA)
                    return;

                // Regenerate 6% of Total Mana Every 3 secs
                int32 EffectBasePoints0 = unitTarget->GetMaxPower(POWER_MANA)  * damage / 100;
                m_caster->CastCustomSpell(unitTarget,39609,&EffectBasePoints0,NULL,NULL,true,NULL,NULL,m_originalCasterGUID);
                return;
            }

            break;
    }
}

void Spell::EffectTriggerSpellWithValue(uint32 i)
{
    uint32 triggered_spell_id = m_spellInfo->EffectTriggerSpell[i];

    // normal case
    SpellEntry const *spellInfo = sSpellStore.LookupEntry( triggered_spell_id );

    if(!spellInfo)
    {
        sLog.outError("EffectTriggerSpellWithValue of spell %u: triggering unknown spell id %i\n", m_spellInfo->Id,triggered_spell_id);
        return;
    }

    int32 bp = damage;
    m_caster->CastCustomSpell(unitTarget,triggered_spell_id,&bp,&bp,&bp,true,NULL,NULL,m_originalCasterGUID);
}

void Spell::EffectTriggerSpell(uint32 i)
{
    uint32 triggered_spell_id = m_spellInfo->EffectTriggerSpell[i];

    // special cases
    switch(triggered_spell_id)
    {
        // just skip
        case 23770:                                         // Sayge's Dark Fortune of *
            // not exist, common cooldown can be implemented in scripts if need.
            return;
        // Brittle Armor - (need add max stack of 24575 Brittle Armor)
        case 29284:
        {
            const SpellEntry *spell = sSpellStore.LookupEntry(24575);
            if (!spell)
                return;

            for (int i=0; i < spell->StackAmount; ++i)
                m_caster->CastSpell(unitTarget,spell->Id, true, m_CastItem, NULL, m_originalCasterGUID);
            return;
        }
        // Mercurial Shield - (need add max stack of 26464 Mercurial Shield)
        case 29286:
        {
            const SpellEntry *spell = sSpellStore.LookupEntry(26464);
            if (!spell)
                return;

            for (int i=0; i < spell->StackAmount; ++i)
                m_caster->CastSpell(unitTarget,spell->Id, true, m_CastItem, NULL, m_originalCasterGUID);
            return;
        }
        // Righteous Defense
        case 31980:
        {
            m_caster->CastSpell(unitTarget, 31790, true,m_CastItem,NULL,m_originalCasterGUID);
            return;
        }
        // Cloak of Shadows
        case 35729 :
        {
            Unit::AuraMap& Auras = m_caster->GetAuras();
            for(Unit::AuraMap::iterator iter = Auras.begin(); iter != Auras.end(); ++iter)
            {
                // remove all harmful spells on you... 
                if( // ignore positive and passive auras
                    !iter->second->IsPositive() && !iter->second->IsPassive()    &&
                    // ignore physical auras
                    (GetSpellSchoolMask(iter->second->GetSpellProto()) & SPELL_SCHOOL_MASK_NORMAL)==0 &&
                    // ignore immunity persistent spells
                    !( iter->second->GetSpellProto()->AttributesEx & 0x10000 ) )
                {
                    m_caster->RemoveAurasDueToSpell(iter->second->GetSpellProto()->Id);
                    iter = Auras.begin();
                }
            }
            return;
        }
        // Priest Shadowfiend (34433) need apply mana gain trigger aura on pet
        case 41967:
        {
            if (Unit *pet = m_caster->GetPet())
                pet->CastSpell(pet, 28305, true);
            return;
        }
    }

    // normal case
    SpellEntry const *spellInfo = sSpellStore.LookupEntry( triggered_spell_id );

    if(!spellInfo)
    {
        sLog.outError("EffectTriggerSpell of spell %u: triggering unknown spell id %i", m_spellInfo->Id,triggered_spell_id);
        return;
    }

    // some triggered spells require specific equipment
    if(spellInfo->EquippedItemClass >=0 && m_caster->GetTypeId()==TYPEID_PLAYER)
    {
        Player* p_caster = (Player*)m_caster;

        // main hand weapon required
        if(spellInfo->AttributesEx3 & 0x0000000000000400)
        {
            Item* item = ((Player*)m_caster)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

            // skip spell if no weapon in slot or broken
            if(!item || item->IsBroken() )
                return;

            // skip spell if weapon not fit to triggered spell
            if(!item->IsFitToSpellRequirements(spellInfo))
                return;
        }

        // offhand hand weapon required
        if(spellInfo->AttributesEx3 & 0x0000000001000000)
        {
            Item* item = ((Player*)m_caster)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

            // skip spell if no weapon in slot or broken
            if(!item || item->IsBroken() )
                return;

            // skip spell if weapon not fit to triggered spell
            if(!item->IsFitToSpellRequirements(spellInfo))
                return;
        }
    }

    // some triggered spells must be casted instantly (for example, if next effect case instant kill caster)
    bool instant = false;
    for(uint32 j = i+1; j < 3; ++j)
    {
        if(m_spellInfo->Effect[j]==SPELL_EFFECT_INSTAKILL && m_spellInfo->EffectImplicitTargetA[j]==TARGET_SELF)
        {
            instant = true;
            break;
        }
    }

    if(instant)
    {
        // in case multi-targets, spell must be casted one time, at last target in list)
        if (unitTarget && m_UniqueTargetInfo.back().targetGUID==unitTarget->GetGUID())
            m_caster->CastSpell(unitTarget,spellInfo,true,m_CastItem,NULL,m_originalCasterGUID);
    }
    else
        m_TriggerSpells.push_back(spellInfo);
}

void Spell::EffectTeleportUnits(uint32 /*i*/)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if(unitTarget ->isInFlight())
        return;

    // special case for homebind
    if(m_spellInfo->Id == 8690 || m_spellInfo->Id == 556)
    {
        //homebind location is loaded always
        ((Player*)unitTarget)->TeleportTo(((Player*)m_caster)->m_homebindMapId,((Player*)m_caster)->m_homebindX,((Player*)m_caster)->m_homebindY,((Player*)m_caster)->m_homebindZ,unitTarget->GetOrientation());
        return;
    }

    if(m_spellInfo->Id == 36563 && m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        Unit *pTarget = ObjectAccessor::GetUnit(*m_caster, ((Player*)m_caster)->GetSelection());
        if(!pTarget)
            return;

        float _target_x, _target_y, _target_z;
        pTarget->GetClosePoint(_target_x, _target_y, _target_z, CONTACT_DISTANCE + pTarget->GetObjectSize() + m_caster->GetObjectSize(), M_PI);

        if(!pTarget->IsWithinLOS(_target_x,_target_y,_target_z))
            return;

        ((Player*)m_caster)->TeleportTo(m_caster->GetMapId(),_target_x, _target_y, _target_z , pTarget->GetOrientation());
        return;
    }

    SpellTeleport const* st = spellmgr.GetSpellTeleport(m_spellInfo->Id);
    if(!st)
    {
        sLog.outError( "SPELL: unknown Teleport coordinates for spell ID %u\n", m_spellInfo->Id );
        return;
    }

    ((Player*)unitTarget)->TeleportTo(st->target_mapId,st->target_X,st->target_Y,st->target_Z,st->target_Orientation);

    // post effects
    switch ( m_spellInfo->Id )
    {
        // Dimensional Ripper - Everlook
        case 23442:
        {
            int32 r = irand(0, 119);
            if ( r >= 70 )                                  // 7/12 success
            {
                if ( r < 100 )                              // 4/12 evil twin
                    m_caster->CastSpell(m_caster,23445,true);
                else                                        // 1/12 fire
                    m_caster->CastSpell(m_caster,23449,true);
            }
            return;
        }
        // Ultrasafe Transporter: Toshley's Station
        case 36941:
        {
            if ( roll_chance_i(50) )                        // 50% success
            {
                int32 rand_eff = urand(1,7);
                switch ( rand_eff )
                {
                    case 1:
                        // soul split - evil
                        m_caster->CastSpell(m_caster,36900,true);
                        break;
                    case 2:
                        // soul split - good
                        m_caster->CastSpell(m_caster,36901,true);
                        break;
                    case 3:
                        // Increase the size
                        m_caster->CastSpell(m_caster,36895,true);
                        break;
                    case 4:
                        // Decrease the size
                        m_caster->CastSpell(m_caster,36893,true);
                        break;
                    case 5:
                    // Transform
                    {
                        if (((Player*)m_caster)->GetTeam() == ALLIANCE )
                            m_caster->CastSpell(m_caster,36897,true);
                        else
                            m_caster->CastSpell(m_caster,36899,true);
                        break;
                    }
                    case 6:
                        // chicken
                        m_caster->CastSpell(m_caster,36940,true);
                        break;
                    case 7:
                        // evil twin
                        m_caster->CastSpell(m_caster,23445,true);
                        break;
                }
            }
            return;
        }
        // Dimensional Ripper - Area 52
        case 36890:
        {
            if ( roll_chance_i(50) )                        // 50% success
            {
                int32 rand_eff = urand(1,4);
                switch ( rand_eff )
                {
                    case 1:
                        // soul split - evil
                        m_caster->CastSpell(m_caster,36900,true);
                        break;
                    case 2:
                        // soul split - good
                        m_caster->CastSpell(m_caster,36901,true);
                        break;
                    case 3:
                        // Increase the size
                        m_caster->CastSpell(m_caster,36895,true);
                        break;
                    case 4:
                    // Transform
                    {
                        if (((Player*)m_caster)->GetTeam() == ALLIANCE )
                            m_caster->CastSpell(m_caster,36897,true);
                        else
                            m_caster->CastSpell(m_caster,36899,true);
                        break;
                    }
                }
            }
            return;
        }
    }
}

void Spell::EffectApplyAura(uint32 i)
{
    if(!unitTarget)
        return;

    SpellImmuneList const& list = unitTarget->m_spellImmune[IMMUNITY_STATE];
    for(SpellImmuneList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
        if(itr->type == m_spellInfo->EffectApplyAuraName[i])
            return;

    // ghost spell check, allow apply any auras at player loading in ghost mode (will be cleanup after load)
    if( !unitTarget->isAlive() && m_spellInfo->Id != 20584 && m_spellInfo->Id != 8326 &&
        (unitTarget->GetTypeId()!=TYPEID_PLAYER || !((Player*)unitTarget)->GetSession()->PlayerLoading()) )
        return;

    Unit* caster = m_originalCasterGUID ? m_originalCaster : m_caster;
    if(!caster)
        return;

    sLog.outDebug("Spell: Aura is: %u", m_spellInfo->EffectApplyAuraName[i]);

    Aura* Aur = CreateAura(m_spellInfo, i, &m_currentBasePoints[i], unitTarget, caster, m_CastItem);

    if (!Aur->IsPositive() && Aur->GetCasterGUID() != Aur->GetTarget()->GetGUID())
    {
        bool attack = false;
        const SpellEntry* spellEntry = Aur->GetSpellProto();
        switch (Aur->GetModifier()->m_auraname)
        {
            case SPELL_AURA_BIND_SIGHT:
            case SPELL_AURA_MOD_CHARM:
            case SPELL_AURA_FAR_SIGHT:
            case SPELL_AURA_MOD_DETECT_RANGE:
            case SPELL_AURA_AURAS_VISIBLE:
            case SPELL_AURA_MOD_STALKED:
            case SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS:
            case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
            case SPELL_AURA_EMPATHY:
            case SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS:
            case SPELL_AURA_MOD_INVISIBILITY_DETECTION:
                break;
            case SPELL_AURA_MOD_STUN:
                // I'm not sure, if all STUN-Auras prevent attack, therefore the query
                if(!(spellEntry->SpellFamilyName == SPELLFAMILY_ROGUE && spellEntry->SpellFamilyFlags & SPELLFAMILYFLAG_ROGUE_SAP))
                    attack=true;
                break;
            case SPELL_AURA_DUMMY:
                // Don't attack on Arcane Missiles Dummy Aura, attack at hit
                if(spellEntry->SpellFamilyName == SPELLFAMILY_MAGE && (spellEntry->SpellFamilyFlags & 0x800LL))
                    break;

                // If Aura is applied to monster then attack caster
                attack=true;
                break;
            default:
                // If Aura is applied to monster then attack caster
                attack=true;
                break;
        }
        if(attack)
        {
            if( Aur->GetTarget()->GetTypeId() == TYPEID_UNIT && !Aur->GetTarget()->isInCombat() &&
                ((Creature*)Aur->GetTarget())->AI() )
                ((Creature*)Aur->GetTarget())->AI()->AttackStart(caster);

            Aur->GetTarget()->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
        }
    }

    // Update aura duration based at diminishingMod
    {
        // Use Effect mechanic
        DiminishingMechanics mech = Unit::Mechanic2DiminishingMechanics(Aur->GetSpellProto()->EffectMechanic[i]);

        // Use Spell mechanic
        if (mech == DIMINISHING_NONE)
            mech = Unit::Mechanic2DiminishingMechanics(Aur->GetSpellProto()->Mechanic);

        if(mech == DIMINISHING_NONE)
        {
            switch(m_spellInfo->EffectApplyAuraName[i])
            {
                case SPELL_AURA_MOD_CONFUSE:
                    mech = DIMINISHING_MECHANIC_CONFUSE; break;
                case SPELL_AURA_MOD_CHARM:
                case SPELL_AURA_MOD_FEAR:
                    mech = DIMINISHING_MECHANIC_CHARM; break;
                case SPELL_AURA_MOD_STUN:
                    mech = DIMINISHING_MECHANIC_STUN; break;
                case SPELL_AURA_MOD_ROOT:
                    mech = DIMINISHING_MECHANIC_ROOT; break;
                case SPELL_AURA_MOD_DECREASE_SPEED:
                    mech = DIMINISHING_MECHANIC_SNARE; break;
                default: break;
            }
        }

        // Death Coil and Curse of Exhaustion are not diminished.
        if((m_spellInfo->SpellVisual == 64 && m_spellInfo->SpellIconID == 88) ||
            (m_spellInfo->SpellVisual == 185 && m_spellInfo->SpellIconID == 228))
            mech = DIMINISHING_NONE;

        int32 duration = Aur->GetAuraMaxDuration();

        unitTarget->ApplyDiminishingToDuration(mech,duration,caster);

        // if Aura removed and deleted, do not continue.
        if(duration== 0 && !(Aur->IsPermanent()))
        {
            delete Aur;
            return;
        }

        if(duration != Aur->GetAuraMaxDuration())
        {
            Aur->SetAuraMaxDuration(duration);
            Aur->SetAuraDuration(duration);
        }
    }

    bool added = unitTarget->AddAura(Aur);

    // Aura not added and deleted in AddAura call;
    if (!added)
        return;

    // found crash at character loading, broken pointer to Aur...
    // Aur was deleted in AddAura()...
    if(!Aur)
        return;

    // TODO Make a way so it works for every related spell!
    if(unitTarget->GetTypeId()==TYPEID_PLAYER)              // Negative buff should only be applied on players
    {
        uint32 spellId = 0;
                                                            // Power Word: Shield
        if (m_spellInfo->SpellFamilyName == SPELLFAMILY_PRIEST && m_spellInfo->SpellFamilyFlags & 1)
            spellId = 6788;                                 // Weakened Soul
        else if(IsMechanicInvulnerabilityImmunityToSpell(m_spellInfo))
            spellId = 25771;                                // Forbearance
        else if (m_spellInfo->Mechanic == MECHANIC_BANDAGE) // Bandages
            spellId = 11196;                                // Recently Bandaged
                                                            // Ice Block
        else if (m_spellInfo->SpellFamilyName == SPELLFAMILY_MAGE && m_spellInfo->SpellFamilyFlags & 0x8000000000LL)
            spellId = SPELLID_MAGE_HYPOTHERMIA;             // Hypothermia
        else if( (m_spellInfo->AttributesEx & 0x20) && (m_spellInfo->AttributesEx2 & 0x20000) )
            spellId = 23230;                                // Blood Fury - Healing Reduction

        SpellEntry const *AdditionalSpellInfo = sSpellStore.LookupEntry(spellId);
        if (AdditionalSpellInfo)
        {
            // applied at target by target
            Aura* AdditionalAura = CreateAura(AdditionalSpellInfo, 0, &m_currentBasePoints[0], unitTarget,unitTarget, 0);
            unitTarget->AddAura(AdditionalAura);
            sLog.outDebug("Spell: Additional Aura is: %u", AdditionalSpellInfo->EffectApplyAuraName[0]);
        }
    }

    // Prayer of Mending (jump animation), we need formal caster instead original for correct animation
    if( m_spellInfo->SpellFamilyName == SPELLFAMILY_PRIEST && (m_spellInfo->SpellFamilyFlags & 0x00002000000000LL))
        m_caster->CastSpell(unitTarget,41637,true,NULL,Aur);
}

void Spell::EffectUnlearnSpecialization( uint32 i )
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)unitTarget;
    uint32 spellToUnlearn = m_spellInfo->EffectTriggerSpell[i];

    _player->removeSpell(spellToUnlearn);

    sLog.outDebug( "Spell: Player %u have unlearned spell %u from NpcGUID: %u", _player->GetGUIDLow(), spellToUnlearn, m_caster->GetGUIDLow() );
}

void Spell::EffectManaDrain(uint32 i)
{
    if(m_spellInfo->EffectMiscValue[i] < 0 || m_spellInfo->EffectMiscValue[i] >= MAX_POWERS)
        return;

    Powers drain_power = Powers(m_spellInfo->EffectMiscValue[i]);

    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;
    if(unitTarget->getPowerType() != drain_power)
        return;
    if(damage < 0)
        return;

    uint32 curPower = unitTarget->GetPower(drain_power);

    int32 new_damage;

    // resilience reduce mana draining effect (added in 2.4)
    uint32 power = damage;
    if ( drain_power == POWER_MANA && unitTarget->GetTypeId() == TYPEID_PLAYER )
        power -= ((Player*)unitTarget)->GetDotDamageReduction(damage);
    
    if(curPower < power)
        new_damage = curPower;
    else
        new_damage = power;

    unitTarget->ModifyPower(drain_power,-new_damage);

    if(drain_power == POWER_MANA)
    {
        float manaMultiplier = m_spellInfo->EffectMultipleValue[i];
        if(manaMultiplier==0)
            manaMultiplier = 1;

        if(Player *modOwner = m_caster->GetSpellModOwner())
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_MULTIPLE_VALUE, manaMultiplier);

        m_caster->ModifyPower(POWER_MANA,uint32(new_damage*manaMultiplier));
    }
}

void Spell::EffectSendEvent(uint32 EffectIndex)
{
    if (m_caster->GetTypeId() == TYPEID_PLAYER && ((Player*)m_caster)->InBattleGround())
    {
        BattleGround* bg = ((Player *)m_caster)->GetBattleGround();
        if(bg && bg->GetStatus() == STATUS_IN_PROGRESS)
        {
            switch(m_spellInfo->Id)
            {
                case 23333:                                 // Pickup Horde Flag
                    /*do not uncomment .
                    if(bg->GetTypeID()==BATTLEGROUND_WS)
                        bg->EventPlayerClickedOnFlag((Player*)m_caster, this->gameObjTarget);
                    sLog.outDebug("Send Event Horde Flag Picked Up");
                    break;
                    /* not used :
                    case 23334:                                 // Drop Horde Flag
                        if(bg->GetTypeID()==BATTLEGROUND_WS)
                            bg->EventPlayerDroppedFlag((Player*)m_caster);
                        sLog.outDebug("Drop Horde Flag");
                        break;
                    */
                case 23335:                                 // Pickup Alliance Flag
                    /*do not uncomment ... (it will cause crash, because of null targetobject!) anyway this is a bad way to call that event, because it would cause recursion
                    if(bg->GetTypeID()==BATTLEGROUND_WS)
                        bg->EventPlayerClickedOnFlag((Player*)m_caster, this->gameObjTarget);
                    sLog.outDebug("Send Event Alliance Flag Picked Up");
                    break;
                    /* not used :
                    case 23336:                                 // Drop Alliance Flag
                        if(bg->GetTypeID()==BATTLEGROUND_WS)
                            bg->EventPlayerDroppedFlag((Player*)m_caster);
                        sLog.outDebug("Drop Alliance Flag");
                        break;
                    case 23385:                                 // Alliance Flag Returns
                        if(bg->GetTypeID()==BATTLEGROUND_WS)
                            bg->EventPlayerClickedOnFlag((Player*)m_caster, this->gameObjTarget);
                        sLog.outDebug("Alliance Flag Returned");
                        break;
                    case 23386:                                   // Horde Flag Returns
                        if(bg->GetTypeID()==BATTLEGROUND_WS)
                            bg->EventPlayerClickedOnFlag((Player*)m_caster, this->gameObjTarget);
                        sLog.outDebug("Horde Flag Returned");
                        break;*/
                case 34976:
                    /*
                    if(bg->GetTypeID()==BATTLEGROUND_EY)
                        bg->EventPlayerClickedOnFlag((Player*)m_caster, this->gameObjTarget);
                    */
                    break;
                default:
                    sLog.outDebug("Unknown spellid %u in BG event", m_spellInfo->Id);
                    break;
            }
        }
    }
    sLog.outDebug("Spell ScriptStart %u for spellid %u in EffectSendEvent ", m_spellInfo->EffectMiscValue[EffectIndex], m_spellInfo->Id);
    sWorld.ScriptsStart(sEventScripts, m_spellInfo->EffectMiscValue[EffectIndex], m_caster, focusObject);
}

void Spell::EffectPowerBurn(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;
    if(unitTarget->getPowerType()!=POWER_MANA)
        return;
    if(damage < 0)
        return;

    int32 curPower = int32(unitTarget->GetPower(POWER_MANA));

    // resilience reduce mana draining effect (added in 2.4)
    uint32 power = damage;
    if(unitTarget->GetTypeId() == TYPEID_PLAYER)
        power -= ((Player*)unitTarget)->GetDotDamageReduction(damage);

    int32 new_damage = (curPower < power) ? curPower : power;

    unitTarget->ModifyPower(POWER_MANA,-new_damage);
    float multiplier = m_spellInfo->EffectMultipleValue[i];
    if(Player *modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_MULTIPLE_VALUE, multiplier);

    new_damage = int32(new_damage*multiplier);
    m_caster->SpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, new_damage, m_IsTriggeredSpell, true);
}

void Spell::EffectHeal( uint32 /*i*/ )
{
    if( unitTarget && unitTarget->isAlive() && damage >= 0)
    {
        // Try to get original caster
        Unit *caster = m_originalCasterGUID ? m_originalCaster : m_caster;

        // Skip if m_originalCaster not available
        if (!caster)
            return;

        // Swiftmend - consumes Regrowth or Rejuvenation
        if (m_spellInfo->TargetAuraState == AURA_STATE_SWIFTMEND)
        {
            Unit::AuraList const& RejorRegr = unitTarget->GetAurasByType(SPELL_AURA_PERIODIC_HEAL);
            if(!RejorRegr.empty())
            {
                // find most short by duration
                Unit::AuraList::const_iterator short_itr = RejorRegr.begin();
                for(Unit::AuraList::const_iterator i = RejorRegr.begin(); i != RejorRegr.end(); ++i)
                {
                    if((*i)->GetSpellProto()->SpellFamilyName == SPELLFAMILY_DRUID
                        && ((*i)->GetSpellProto()->SpellFamilyFlags == 0x40 || (*i)->GetSpellProto()->SpellFamilyFlags == 0x10) )
                    {
                        if((*i)->GetAuraDuration() < (*short_itr)->GetAuraDuration())
                            short_itr = i;
                    }
                }

                switch((*short_itr)->GetSpellProto()->SpellFamilyFlags)
                {
                    case 16:                                //Rejuvenation
                        damage += (*short_itr)->GetModifier()->m_amount * 4;
                        unitTarget->RemoveAurasDueToSpell((*short_itr)->GetId());
                        break;
                    case 64:                                //Regrowth
                        damage += (*short_itr)->GetModifier()->m_amount * 6;
                        unitTarget->RemoveAurasDueToSpell((*short_itr)->GetId());
                        break;
                    default:
                        break;
                }
            }
        }
        uint32 addhealth = caster->SpellHealingBonus(m_spellInfo, uint32(damage),HEAL, unitTarget);
        bool crit = caster->isSpellCrit(unitTarget, m_spellInfo, m_spellSchoolMask, m_attackType);
        if (crit)
            addhealth = caster->SpellCriticalBonus(m_spellInfo, addhealth, unitTarget);
        caster->SendHealSpellLog(unitTarget, m_spellInfo->Id, addhealth, crit);

        int32 gain = unitTarget->ModifyHealth( int32(addhealth) );
        unitTarget->getHostilRefManager().threatAssist(m_caster, float(gain) * 0.5f, m_spellInfo);

        if(caster->GetTypeId()==TYPEID_PLAYER)
            if(BattleGround *bg = ((Player*)caster)->GetBattleGround())
                bg->UpdatePlayerScore(((Player*)caster), SCORE_HEALING_DONE, gain);

        // ignore item heals
        if(m_CastItem)
            return;

        uint32 procHealer = PROC_FLAG_HEAL;
        if (crit)
            procHealer |= PROC_FLAG_CRIT_HEAL;

        m_caster->ProcDamageAndSpell(unitTarget,procHealer,PROC_FLAG_HEALED,addhealth,m_spellInfo,m_IsTriggeredSpell);
    }
}

void Spell::EffectHealMechanical( uint32 /*i*/ )
{
    // Mechanic creature type should be correctly checked by targetCreatureType field
    if( unitTarget && unitTarget->isAlive() && damage >= 0)
    {
        // Try to get original caster
        Unit *caster = m_originalCasterGUID ? m_originalCaster : m_caster;

        // Skip if m_originalCaster not available
        if (!caster)
            return;

        uint32 addhealth = caster->SpellHealingBonus(m_spellInfo, uint32(damage), HEAL, unitTarget);
        caster->SendHealSpellLog(unitTarget, m_spellInfo->Id, addhealth, false);
        unitTarget->ModifyHealth( int32(damage) );
    }
}

void Spell::EffectHealthLeech(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    if(damage < 0)
        return;

    sLog.outDebug("HealthLeech :%i", damage);

    float multiplier = m_spellInfo->EffectMultipleValue[i];

    if(Player *modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_MULTIPLE_VALUE, multiplier);

    int32 new_damage = int32(damage*multiplier);
    uint32 curHealth = unitTarget->GetHealth();
    new_damage = m_caster->SpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, new_damage, m_IsTriggeredSpell, true);
    if(curHealth < new_damage)
        new_damage = curHealth;

    if(m_caster->isAlive())
    {
        m_caster->ModifyHealth(new_damage);

        if(m_caster->GetTypeId() == TYPEID_PLAYER)
            m_caster->SendHealSpellLog(m_caster, m_spellInfo->Id, uint32(new_damage));
    }
}

void Spell::DoCreateItem(uint32 i, uint32 itemtype)
{
    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = (Player*)unitTarget;

    uint32 newitemid = itemtype;
    ItemPrototype const *pProto = objmgr.GetItemPrototype( newitemid );
    if(!pProto)
    {
        player->SendEquipError( EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL );
        return;
    }

    uint32 num_to_add;

    // TODO: maybe all this can be replaced by using correct calculated `damage` value
    if(pProto->Class != ITEM_CLASS_CONSUMABLE || m_spellInfo->SpellFamilyName != SPELLFAMILY_MAGE)
    {
        int32 basePoints = m_currentBasePoints[i];
        int32 randomPoints = m_spellInfo->EffectDieSides[i];
        if (randomPoints)
            num_to_add = basePoints + irand(1, randomPoints);
        else
            num_to_add = basePoints + 1;
    }
    else if (pProto->MaxCount == 1)
        num_to_add = 1;
    else if(player->getLevel() >= m_spellInfo->spellLevel)
    {
        int32 basePoints = m_currentBasePoints[i];
        float pointPerLevel = m_spellInfo->EffectRealPointsPerLevel[i];
        num_to_add = basePoints + 1 + uint32((player->getLevel() - m_spellInfo->spellLevel)*pointPerLevel);
    }
    else
        num_to_add = 2;

    if (num_to_add < 1)
        num_to_add = 1;
    if (num_to_add > pProto->Stackable)
        num_to_add = pProto->Stackable;

    // init items_count to 1, since 1 item will be created regardless of specialization
    int items_count=1;
    // the chance to create additional items
    float additionalCreateChance=0.0f;
    // the maximum number of created additional items
    uint8 additionalMaxNum=0;
    // get the chance and maximum number for creating extra items
    if ( canCreateExtraItems(player, m_spellInfo->Id, additionalCreateChance, additionalMaxNum) )
    {
        // roll with this chance till we roll not to create or we create the max num
        while ( roll_chance_f(additionalCreateChance) && items_count<=additionalMaxNum )
            ++items_count;
    }

    // really will be created more items
    num_to_add *= items_count;

    // can the player store the new item?
    ItemPosCountVec dest;
    uint32 no_space = 0;
    uint8 msg = player->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, newitemid, num_to_add, &no_space );
    if( msg != EQUIP_ERR_OK )
    {
        if( msg == EQUIP_ERR_INVENTORY_FULL )                   // convert to possibl� store amount
            num_to_add -= no_space;
        else
        {
            player->SendEquipError( msg, NULL, NULL );
            return;
        }
    }

    if(num_to_add)
    {
        // create the new item and store it
        Item* pItem = player->StoreNewItem( dest, newitemid, true, Item::GenerateItemRandomPropertyId(newitemid));

        // was it successful? return error if not
        if(!pItem)
        {
            player->SendEquipError( EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL );
            return;
        }

        // set the "Crafted by ..." property of the item
        if( pItem->GetProto()->Class != ITEM_CLASS_CONSUMABLE && pItem->GetProto()->Class != ITEM_CLASS_QUEST)
            pItem->SetUInt32Value(ITEM_FIELD_CREATOR,player->GetGUIDLow());

        // send info to the client
        if(pItem)
            player->SendNewItem(pItem, num_to_add, true, true);

        // we succeeded in creating at least one item, so a levelup is possible
        player->UpdateCraftSkill(m_spellInfo->Id);
    }

    // for battleground marks send by mail if not add all expected
    if(no_space > 0 )
    {
        BattleGroundTypeId bgType;
        switch(m_spellInfo->Id)
        {
            case SPELL_AV_MARK_WINNER:
            case SPELL_AV_MARK_LOSER:
                bgType = BATTLEGROUND_AV;
                break;
            case SPELL_WS_MARK_WINNER:
            case SPELL_WS_MARK_LOSER:
                bgType = BATTLEGROUND_WS;
                break;
            case SPELL_AB_MARK_WINNER:
            case SPELL_AB_MARK_LOSER:
                bgType = BATTLEGROUND_AB;
                break;
            default:
                return;
        }

        if(BattleGround* bg = sBattleGroundMgr.GetBattleGround(bgType))
            bg->SendRewardMarkByMail(player,newitemid,no_space);
    }
}

void Spell::EffectCreateItem(uint32 i)
{
    DoCreateItem(i,m_spellInfo->EffectItemType[i]);
}

void Spell::EffectPersistentAA(uint32 i)
{
    float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

    if(Player* modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_RADIUS, radius);

    int32 duration = GetSpellDuration(m_spellInfo);
    DynamicObject* dynObj = new DynamicObject(m_caster);
    if(!dynObj->Create(objmgr.GenerateLowGuid(HIGHGUID_DYNAMICOBJECT), m_caster, m_spellInfo->Id, i, m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, duration, radius))
    {
        delete dynObj;
        return;
    }
    dynObj->SetUInt32Value(OBJECT_FIELD_TYPE, 65);
    dynObj->SetUInt32Value(GAMEOBJECT_DISPLAYID, 368003);
    dynObj->SetUInt32Value(DYNAMICOBJECT_BYTES, 0x01eeeeee);
    m_caster->AddDynObject(dynObj);
    MapManager::Instance().GetMap(dynObj->GetMapId(), dynObj)->Add(dynObj);
}

void Spell::EffectEnergize(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    if(m_spellInfo->EffectMiscValue[i] < 0 || m_spellInfo->EffectMiscValue[i] >= MAX_POWERS)
        return;

    // Some level depends spells
    int multipler  = 0;
    int level_diff = 0;
    switch (m_spellInfo->Id)
    {
        // Restore Energy
        case 9512:
            level_diff = m_caster->getLevel() - 40;
            multipler  = 2;
            break;
        // Blood Fury
        case 24571:
            level_diff = m_caster->getLevel() - 60;
            multipler  = 10;
            break;
        // Burst of Energy
        case 24532:
            level_diff = m_caster->getLevel() - 60;
            multipler  = 4;
            break;
        default:
            break;
    }

    if (level_diff > 0)
        damage -= multipler * level_diff;

    if(damage < 0)
        return;

    Powers power = Powers(m_spellInfo->EffectMiscValue[i]);

    if(unitTarget->GetMaxPower(power) == 0)
        return;

    unitTarget->ModifyPower(power,damage);
    m_caster->SendEnergizeSpellLog(unitTarget, m_spellInfo->Id, damage, power);
}

void Spell::SendLoot(uint64 guid, LootType loottype)
{
    Player* player = (Player*)m_caster;
    if (!player)
        return;

    if (gameObjTarget)
    {
        switch (gameObjTarget->GetGoType())
        {
            case GAMEOBJECT_TYPE_DOOR:
            case GAMEOBJECT_TYPE_BUTTON:
                gameObjTarget->UseDoorOrButton();
                sWorld.ScriptsStart(sGameObjectScripts, gameObjTarget->GetDBTableGUIDLow(), player, gameObjTarget);
                return;

            case GAMEOBJECT_TYPE_QUESTGIVER:
                // start or end quest
                player->PrepareQuestMenu(guid);
                player->SendPreparedQuest(guid);
                return;

            case GAMEOBJECT_TYPE_SPELL_FOCUS:
                // triggering linked GO
                if(uint32 trapEntry = gameObjTarget->GetGOInfo()->spellFocus.linkedTrapId)
                    gameObjTarget->TriggeringLinkedGameObject(trapEntry,m_caster);
                return;

            case GAMEOBJECT_TYPE_GOOBER:
                // goober_scripts can be triggered if the player don't have the quest
                if (gameObjTarget->GetGOInfo()->goober.eventId)
                {
                    sLog.outDebug("Goober ScriptStart id %u for GO %u", gameObjTarget->GetGOInfo()->goober.eventId,gameObjTarget->GetDBTableGUIDLow());
                    sWorld.ScriptsStart(sEventScripts, gameObjTarget->GetGOInfo()->goober.eventId, player, gameObjTarget);
                }

                // cast goober spell
                if (gameObjTarget->GetGOInfo()->goober.questId)
                    ///Quest require to be active for GO using
                    if(player->GetQuestStatus(gameObjTarget->GetGOInfo()->goober.questId) != QUEST_STATUS_INCOMPLETE)
                        return;

                gameObjTarget->AddUniqueUse(player);
                gameObjTarget->SetLootState(GO_JUST_DEACTIVATED);
                player->CastedCreatureOrGO(gameObjTarget->GetEntry(), gameObjTarget->GetGUID(), 0);

                // triggering linked GO
                if(uint32 trapEntry = gameObjTarget->GetGOInfo()->goober.linkedTrapId)
                    gameObjTarget->TriggeringLinkedGameObject(trapEntry,m_caster);

                return;

            case GAMEOBJECT_TYPE_CHEST:
                // TODO: possible must be moved to loot release (in different from linked triggering)
                if (gameObjTarget->GetGOInfo()->chest.eventId)
                {
                    sLog.outDebug("Chest ScriptStart id %u for GO %u", gameObjTarget->GetGOInfo()->chest.eventId,gameObjTarget->GetDBTableGUIDLow());
                    sWorld.ScriptsStart(sEventScripts, gameObjTarget->GetGOInfo()->chest.eventId, player, gameObjTarget);
                }

                // triggering linked GO
                if(uint32 trapEntry = gameObjTarget->GetGOInfo()->chest.linkedTrapId)
                    gameObjTarget->TriggeringLinkedGameObject(trapEntry,m_caster);

                // Don't return, let loots been taken
        }
    }

    // Send loot
    player->SendLoot(guid, loottype);
}

void Spell::EffectOpenLock(uint32 /*i*/)
{
    if(!m_caster || m_caster->GetTypeId() != TYPEID_PLAYER)
    {
        sLog.outDebug( "WORLD: Open Lock - No Player Caster!");
        return;
    }

    Player* player = (Player*)m_caster;

    LootType loottype = LOOT_CORPSE;
    uint32 lockId = 0;
    uint64 guid = 0;

    // Get lockId
    if(gameObjTarget)
    {
        // Arathi Basin banner opening !
        BattleGround *bg = player->GetBattleGround();
        if (gameObjTarget->GetGOInfo()->type == GAMEOBJECT_TYPE_BUTTON && gameObjTarget->GetGOInfo()->button.isBattlegroundObject ||
            gameObjTarget->GetGOInfo()->type == GAMEOBJECT_TYPE_GOOBER && gameObjTarget->GetGOInfo()->goober.isBattlegroundObject )
        {
            if( player->InBattleGround() &&                 // in battleground
                !player->IsMounted() &&                     // not mounted
                !player->HasStealthAura() &&                // not stealthed
                !player->HasInvisibilityAura() &&           // not invisible
                player->isAlive())                          // live player
            {
                // check if it's correct bg
                if(bg && bg->GetTypeID() == BATTLEGROUND_AB)
                    bg->EventPlayerClickedOnFlag(player, gameObjTarget);
                return;
            }
        }
        else if (gameObjTarget->GetGOInfo()->type == GAMEOBJECT_TYPE_FLAGSTAND)
        {
            if(bg && bg->GetTypeID() == BATTLEGROUND_EY)
                bg->EventPlayerClickedOnFlag(player, gameObjTarget);
            return;
        }
        lockId = gameObjTarget->GetLockId();
        guid = gameObjTarget->GetGUID();
    }
    else if(itemTarget)
    {
        lockId = itemTarget->GetProto()->LockID;
        guid = itemTarget->GetGUID();
    }
    else
    {
        sLog.outDebug( "WORLD: Open Lock - No GameObject/Item Target!");
        return;
    }

    if(!lockId)                                             // possible case for GO and maybe for items.
    {
        SendLoot(guid, loottype);
        return;
    }

    // Get LockInfo
    LockEntry const *lockInfo = sLockStore.LookupEntry(lockId);

    if (!lockInfo)
    {
        sLog.outError( "Spell::EffectOpenLock: %s [guid = %u] has an unknown lockId: %u!",
            (gameObjTarget ? "gameobject" : "item"), GUID_LOPART(guid), lockId);
        SendCastResult(SPELL_FAILED_BAD_TARGETS);
        return;
    }

    // check key
    for(int i = 0; i < 5; ++i)
    {
        // type==1 This means lockInfo->key[i] is an item
        if(lockInfo->keytype[i]==LOCK_KEY_ITEM && lockInfo->key[i] && m_CastItem && m_CastItem->GetEntry()==lockInfo->key[i])
        {
            SendLoot(guid, loottype);
            return;
        }
    }

    uint32 SkillId = 0;
    // Check and skill-up skill
    if( m_spellInfo->Effect[1] == SPELL_EFFECT_SKILL )
        SkillId = m_spellInfo->EffectMiscValue[1];
                                                            // pickpocketing spells
    else if( m_spellInfo->EffectMiscValue[0] == LOCKTYPE_PICKLOCK )
        SkillId = SKILL_LOCKPICKING;

    // skill bonus provided by casting spell (mostly item spells)
    uint32 spellSkillBonus = uint32(m_currentBasePoints[0]+1);

    uint32 reqSkillValue = lockInfo->requiredminingskill;

    if(lockInfo->requiredlockskill)                         // required pick lock skill applying
    {
        if(SkillId != SKILL_LOCKPICKING)                    // wrong skill (cheating?)
        {
            SendCastResult(SPELL_FAILED_FIZZLE);
            return;
        }

        reqSkillValue = lockInfo->requiredlockskill;
    }
    else if(SkillId == SKILL_LOCKPICKING)                   // apply picklock skill to wrong target
    {
        SendCastResult(SPELL_FAILED_BAD_TARGETS);
        return;
    }

    if ( SkillId )
    {
        loottype = LOOT_SKINNING;
        if ( player->GetSkillValue(SkillId) + spellSkillBonus < reqSkillValue )
        {
            SendCastResult(SPELL_FAILED_LOW_CASTLEVEL);
            return;
        }

        // update skill if really known
        uint32 SkillValue = player->GetPureSkillValue(SkillId);
        if(SkillValue)                                      // non only item base skill
        {
            if(gameObjTarget)
            {
                // Allow one skill-up until respawned
                if ( !gameObjTarget->IsInSkillupList( player->GetGUIDLow() ) &&
                    player->UpdateGatherSkill(SkillId, SkillValue, reqSkillValue) )
                    gameObjTarget->AddToSkillupList( player->GetGUIDLow() );
            }
            else if(itemTarget)
            {
                // Do one skill-up
                uint32 SkillValue = player->GetPureSkillValue(SkillId);
                player->UpdateGatherSkill(SkillId, SkillValue, reqSkillValue);
            }
        }
    }

    SendLoot(guid, loottype);
}

void Spell::EffectSummonChangeItem(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *player = (Player*)m_caster;

    // applied only to using item
    if(!m_CastItem)
        return;

    // ... only to item in own inventory/bank/equip_slot
    if(m_CastItem->GetOwnerGUID()!=player->GetGUID())
        return;

    uint32 newitemid = m_spellInfo->EffectItemType[i];
    if(!newitemid)
        return;

    uint16 pos = m_CastItem->GetPos();

    Item *pNewItem = Item::CreateItem( newitemid, 1, player);
    if( !pNewItem )
        return;

    if( player->IsInventoryPos( pos ) )
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreItem( m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), dest, pNewItem, true );
        if( msg == EQUIP_ERR_OK )
        {
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(),true);

            // prevent crash at access and unexpected charges counting with item update queue corrupt
            if(m_CastItem==m_targets.getItemTarget())
                m_targets.setItemTarget(NULL);

            m_CastItem = NULL;

            player->StoreItem( dest, pNewItem, true);
            return;
        }
    }
    else if( player->IsBankPos ( pos ) )
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanBankItem( m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), dest, pNewItem, true );
        if( msg == EQUIP_ERR_OK )
        {
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(),true);

            // prevent crash at access and unexpected charges counting with item update queue corrupt
            if(m_CastItem==m_targets.getItemTarget())
                m_targets.setItemTarget(NULL);

            m_CastItem = NULL;

            player->BankItem( dest, pNewItem, true);
            return;
        }
    }
    else if( player->IsEquipmentPos ( pos ) )
    {
        uint16 dest;
        uint8 msg = player->CanEquipItem( m_CastItem->GetSlot(), dest, pNewItem, true );
        if( msg == EQUIP_ERR_OK )
        {
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(),true);

            // prevent crash at access and unexpected charges counting with item update queue corrupt
            if(m_CastItem==m_targets.getItemTarget())
                m_targets.setItemTarget(NULL);

            m_CastItem = NULL;

            player->EquipItem( dest, pNewItem, true);
            player->AutoUnequipOffhandIfNeed();
            return;
        }
    }

    // fail
    delete pNewItem;
}

void Spell::EffectOpenSecretSafe(uint32 i)
{
    EffectOpenLock(i);                                      //no difference for now
}

void Spell::EffectProficiency(uint32 /*i*/)
{
    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    Player *p_target = (Player*)unitTarget;

    uint32 subClassMask = m_spellInfo->EquippedItemSubClassMask;
    if(m_spellInfo->EquippedItemClass == 2 && !(p_target->GetWeaponProficiency() & subClassMask))
    {
        p_target->AddWeaponProficiency(subClassMask);
        p_target->SendProficiency(uint8(0x02),p_target->GetWeaponProficiency());
    }
    if(m_spellInfo->EquippedItemClass == 4 && !(p_target->GetArmorProficiency() & subClassMask))
    {
        p_target->AddArmorProficiency(subClassMask);
        p_target->SendProficiency(uint8(0x04),p_target->GetArmorProficiency());
    }
}

void Spell::EffectApplyAA(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    AreaAura* Aur = new AreaAura(m_spellInfo, i, &m_currentBasePoints[i], unitTarget, m_caster);
    unitTarget->AddAura(Aur);
}

void Spell::EffectSummon(uint32 i)
{
    if(m_caster->GetPetGUID())
        return;

    if(!unitTarget)
        return;
    uint32 pet_entry = m_spellInfo->EffectMiscValue[i];
    if(!pet_entry)
        return;
    uint32 level = m_caster->getLevel();
    Pet* spawnCreature = new Pet(m_caster, SUMMON_PET);

    if(spawnCreature->LoadPetFromDB(m_caster,pet_entry))
    {
        // set timer for unsummon
        int32 duration = GetSpellDuration(m_spellInfo);
        if(duration > 0)
            spawnCreature->SetDuration(duration);

        return;
    }

    // before caster
    float x,y,z;
    m_caster->GetClosePoint(x,y,z);

    uint32 pet_number = objmgr.GeneratePetNumber();
    if(!spawnCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_PET),
        m_caster->GetMapId(),x,y,z,-m_caster->GetOrientation(),
        m_spellInfo->EffectMiscValue[i], pet_number))
    {
        sLog.outErrorDb("Spell::EffectSummon: no such creature entry %u",m_spellInfo->EffectMiscValue[i]);
        delete spawnCreature;
        return;
    }

    // set timer for unsummon
    int32 duration = GetSpellDuration(m_spellInfo);
    if(duration > 0)
        spawnCreature->SetDuration(duration);

    spawnCreature->SetUInt64Value(UNIT_FIELD_SUMMONEDBY,m_caster->GetGUID());
    spawnCreature->SetUInt32Value(UNIT_NPC_FLAGS , 0);
    spawnCreature->setPowerType(POWER_MANA);
    spawnCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,m_caster->getFaction());
    spawnCreature->SetUInt32Value(UNIT_FIELD_FLAGS,0);
    spawnCreature->SetUInt32Value(UNIT_FIELD_BYTES_0,2048);
    spawnCreature->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
    spawnCreature->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP,0);
    spawnCreature->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE,0);
    spawnCreature->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP,1000);
    spawnCreature->SetUInt64Value(UNIT_FIELD_CREATEDBY, m_caster->GetGUID());
    spawnCreature->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);

    spawnCreature->InitStatsForLevel(level);

    spawnCreature->GetCharmInfo()->SetPetNumber(pet_number, false);

    spawnCreature->AIM_Initialize();
    spawnCreature->InitPetCreateSpells();
    spawnCreature->SetHealth(spawnCreature->GetMaxHealth());
    spawnCreature->SetPower(POWER_MANA, spawnCreature->GetMaxPower(POWER_MANA));

    std::string name;
    if(m_caster->GetTypeId() == TYPEID_PLAYER)
        name = ((Player*)m_caster)->GetName();
    else
        name = ((Creature*)m_caster)->GetCreatureInfo()->Name;
    name.append(petTypeSuffix[spawnCreature->getPetType()]);
    spawnCreature->SetName( name );

    MapManager::Instance().GetMap(m_caster->GetMapId(), m_caster)->Add((Creature*)spawnCreature);

    if(m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        m_caster->SetPet(spawnCreature);
        spawnCreature->GetCharmInfo()->SetReactState( REACT_DEFENSIVE );
        spawnCreature->GetCharmInfo()->SetCommandState( COMMAND_FOLLOW );
        spawnCreature->SavePetToDB(PET_SAVE_AS_CURRENT);
        ((Player*)m_caster)->PetSpellInitialize();
    }
}

void Spell::EffectLearnSpell(uint32 i)
{
    if(!unitTarget)
        return;

    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
    {
        if(m_caster->GetTypeId() == TYPEID_PLAYER)
            EffectLearnPetSpell(i);

        return;
    }

    Player *player = (Player*)unitTarget;

    uint32 spellToLearn = (m_spellInfo->Id==SPELL_GENERIC_LEARN) ? damage : m_spellInfo->EffectTriggerSpell[i];
    player->learnSpell(spellToLearn);

    sLog.outDebug( "Spell: Player %u have learned spell %u from NpcGUID=%u", player->GetGUIDLow(), spellToLearn, m_caster->GetGUIDLow() );
}

void Spell::EffectDispel(uint32 i)
{
    if(!unitTarget)
        return;

    for(int n = 0 ; n < damage; ++n)
        if(unitTarget->RemoveFirstAuraByDispel(m_spellInfo->EffectMiscValue[i], m_caster))
            sLog.outDebug("Spell: Removed aura type %u from %s %u (removed by %s %u)",
                m_spellInfo->EffectMiscValue[i],
                unitTarget->GetTypeId()==TYPEID_PLAYER ? "player" : "creature", unitTarget->GetGUIDLow(),
        m_caster->GetTypeId()==TYPEID_PLAYER ? "player" : "creature", m_caster->GetGUIDLow() );
}

void Spell::EffectDualWield(uint32 /*i*/)
{
    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
        ((Player*)unitTarget)->SetCanDualWield(true);
}

void Spell::EffectPull(uint32 /*i*/)
{
    // TODO: create a proper pull towards distract spell center for distract
    sLog.outDebug("WORLD: Spell Effect DUMMY");
}

void Spell::EffectDistract(uint32 /*i*/)
{
    // Check for possible target
    if (!unitTarget)
        return;
    #if 0
    ToDo
        Dont use Relocate here !!!
        int32 levelDiff = int32(m_caster->getLevel()) - int32(unitTarget->getLevel());

    // If there is a victim to distract
    if ((levelDiff > -5) && unitTarget->IsHostileTo(m_caster))
    {
        // Gets angle of distract epicenter / target
        float angle = unitTarget->GetAngle(m_targets.m_destX, m_targets.m_destY);

        //Set proper orientation
        unitTarget->SetOrientation(angle);
        Creature& pUnit = *((Creature *)unitTarget);
        // Update animation if target is creature and player removed previous check

        ((Creature *)unitTarget)->StopMoving() ;
        unitTarget->Relocate(unitTarget->GetPositionX(), unitTarget->GetPositionY(), unitTarget->GetPositionZ(), unitTarget->GetOrientation());
        ((Creature *)unitTarget)->StopMoving() ;
    }
    #endif
}

void Spell::EffectPickPocket(uint32 /*i*/)
{
    if( m_caster->GetTypeId() != TYPEID_PLAYER )
        return;

    // victim must be creature and attackable
    if( !unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT || m_caster->IsFriendlyTo(unitTarget) )
        return;

    // victim have to be alive and humanoid or undead
    if( unitTarget->isAlive() && (unitTarget->GetCreatureTypeMask() &CREATURE_TYPEMASK_HUMANOID_OR_UNDEAD) != 0)
    {
        int32 chance = 10 + int32(m_caster->getLevel()) - int32(unitTarget->getLevel());

        if (chance > irand(0, 19))
        {
            // Stealing successful
            //sLog.outDebug("Sending loot from pickpocket");
            ((Player*)m_caster)->SendLoot(unitTarget->GetGUID(),LOOT_PICKPOCKETING);
        }
        else
        {
            // Reveal action + get attack
            m_caster->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
            if (((Creature*)unitTarget)->AI())
                ((Creature*)unitTarget)->AI()->AttackStart(m_caster);
        }
    }
}

void Spell::EffectAddFarsight(uint32 i)
{
    float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));
    int32 duration = GetSpellDuration(m_spellInfo);
    DynamicObject* dynObj = new DynamicObject(m_caster);
    if(!dynObj->Create(objmgr.GenerateLowGuid(HIGHGUID_DYNAMICOBJECT), m_caster, m_spellInfo->Id, i, m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, duration, radius))
    {
        delete dynObj;
        return;
    }
    dynObj->SetUInt32Value(OBJECT_FIELD_TYPE, 65);
    dynObj->SetUInt32Value(DYNAMICOBJECT_BYTES, 0x80000002);
    m_caster->AddDynObject(dynObj);
    MapManager::Instance().GetMap(dynObj->GetMapId(), dynObj)->Add(dynObj);
    m_caster->SetUInt64Value(PLAYER_FARSIGHT, dynObj->GetGUID());
}

void Spell::EffectSummonWild(uint32 i)
{
    uint32 creature_entry = m_spellInfo->EffectMiscValue[i];
    if(!creature_entry)
        return;

    uint32 level = m_caster->getLevel();

    // level of creature summoned using engineering item based at engineering skill level
    if(m_caster->GetTypeId()==TYPEID_PLAYER && m_CastItem)
    {
        ItemPrototype const *proto = m_CastItem->GetProto();
        if(proto && proto->RequiredSkill == SKILL_ENGINERING)
        {
            uint16 skill202 = ((Player*)m_caster)->GetSkillValue(SKILL_ENGINERING);
            if(skill202)
            {
                level = skill202/5;
            }
        }
    }

    // select center of summon position
    float center_x = m_targets.m_destX;
    float center_y = m_targets.m_destY;
    float center_z = m_targets.m_destZ;

    if (center_x == 0 || center_y == 0 || center_z == 0)
        m_caster->GetClosePoint(center_x, center_y, center_z);

    float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

    int32 amount = damage > 0 ? damage : 1;

    for(int32 count = 0; count < amount; ++count)
    {
        float px, py, pz;
        m_caster->GetRandomPoint(center_x,center_y,center_z,radius,px,py,pz);

        int32 duration = GetSpellDuration(m_spellInfo);

        TempSummonType summonType = (duration == 0) ? TEMPSUMMON_DEAD_DESPAWN : TEMPSUMMON_TIMED_OR_DEAD_DESPAWN;

        m_caster->SummonCreature(creature_entry,px,py,pz,m_caster->GetOrientation(),summonType,duration);
    }
}

void Spell::EffectSummonGuardian(uint32 i)
{
    uint32 pet_entry = m_spellInfo->EffectMiscValue[i];
    if(!pet_entry)
        return;

    // Jewelery statue case (totem like)
    if(m_spellInfo->SpellIconID==2056)
    {
        EffectSummonTotem(i);
        return;
    }

    Pet* old_wild = NULL;

    {
        CellPair p(MaNGOS::ComputeCellPair(m_caster->GetPositionX(), m_caster->GetPositionY()));
        Cell cell(p);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        PetWithIdCheck u_check(m_caster, pet_entry);
        MaNGOS::UnitSearcher<PetWithIdCheck> checker((Unit*&)old_wild, u_check);
        TypeContainerVisitor<MaNGOS::UnitSearcher<PetWithIdCheck>, WorldTypeMapContainer > object_checker(checker);
        CellLock<GridReadGuard> cell_lock(cell, p);
        cell_lock->Visit(cell_lock, object_checker, *MapManager::Instance().GetMap(m_caster->GetMapId(), m_caster));
    }

    if (old_wild)                                           // find old critter, unsummon
    {
        old_wild->Remove(PET_SAVE_AS_DELETED);
        return;
    }
    else                                                    // in another case summon new
    {
        uint32 level = m_caster->getLevel();

        // level of pet summoned using engineering item based at engineering skill level
        if(m_caster->GetTypeId()==TYPEID_PLAYER && m_CastItem)
        {
            ItemPrototype const *proto = m_CastItem->GetProto();
            if(proto && proto->RequiredSkill == SKILL_ENGINERING)
            {
                uint16 skill202 = ((Player*)m_caster)->GetSkillValue(SKILL_ENGINERING);
                if(skill202)
                {
                    level = skill202/5;
                }
            }
        }

        // select center of summon position
        float center_x = m_targets.m_destX;
        float center_y = m_targets.m_destY;
        float center_z = m_targets.m_destZ;

        if (center_x == 0 || center_y == 0 || center_z == 0)
            m_caster->GetClosePoint(center_x, center_y, center_z);

        float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

        int32 amount = damage > 0 ? damage : 1;

        for(int32 count = 0; count < amount; ++count)
        {
            float px, py, pz;
            m_caster->GetRandomPoint(center_x,center_y,center_z,radius,px,py,pz);

            Pet* spawnCreature = new Pet(m_caster, GUARDIAN_PET);

            uint32 pet_number = objmgr.GeneratePetNumber();
            if(!spawnCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_PET),
                m_caster->GetMapId(),px,py,pz,m_caster->GetOrientation(),
                m_spellInfo->EffectMiscValue[i], pet_number))
            {
                sLog.outError("no such creature entry %u",m_spellInfo->EffectMiscValue[i]);
                delete spawnCreature;
                return;
            }

            // set timer for unsummon
            int32 duration = GetSpellDuration(m_spellInfo);
            if(duration > 0)
                spawnCreature->SetDuration(duration);

            spawnCreature->SetUInt64Value(UNIT_FIELD_SUMMONEDBY,m_caster->GetGUID());
            spawnCreature->setPowerType(POWER_MANA);
            spawnCreature->SetMaxPower(POWER_MANA,28 + 10 * level);
            spawnCreature->SetPower(   POWER_MANA,28 + 10 * level);
            spawnCreature->SetUInt32Value(UNIT_NPC_FLAGS , 0);
            spawnCreature->SetMaxHealth( 28 + 30*level);
            spawnCreature->SetHealth(    28 + 30*level);
            spawnCreature->SetLevel(level);
            spawnCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,m_caster->getFaction());
            spawnCreature->SetUInt32Value(UNIT_FIELD_FLAGS,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP,1000);
            spawnCreature->SetUInt64Value(UNIT_FIELD_CREATEDBY, m_caster->GetGUID());
            spawnCreature->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);

            spawnCreature->SetArmor(level*50);
            spawnCreature->AIM_Initialize();

            MapManager::Instance().GetMap(m_caster->GetMapId(), m_caster)->Add((Creature*)spawnCreature);
        }
    }
}

void Spell::EffectTeleUnitsFaceCaster(uint32 i)
{
    if(!unitTarget)
        return;

    if(unitTarget->isInFlight())
        return;

    uint32 mapid = m_caster->GetMapId();
    float dis = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

    float fx,fy,fz;
    m_caster->GetClosePoint(fx,fy,fz,unitTarget->GetObjectSize() + dis);

    if(unitTarget->GetTypeId() == TYPEID_PLAYER)
        ((Player*)unitTarget)->TeleportTo(mapid, fx, fy, fz, -m_caster->GetOrientation(), false);
    else
        MapManager::Instance().GetMap(mapid, m_caster)->CreatureRelocation((Creature*)m_caster, fx, fy, fz, -m_caster->GetOrientation());
}

void Spell::EffectLearnSkill(uint32 i)
{
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if(damage < 0)
        return;

    uint32 skillid =  m_spellInfo->EffectMiscValue[i];
    uint16 skillval = ((Player*)unitTarget)->GetPureSkillValue(skillid);
    ((Player*)unitTarget)->SetSkill(skillid, skillval?skillval:1, damage*75);
}

void Spell::EffectAddHonor(uint32 /*i*/)
{
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    sLog.outDebug("SpellEffect::AddHonor called for spell_id %u , that rewards %d honor points to player: %u", m_spellInfo->Id, this->damage, ((Player*)unitTarget)->GetGUIDLow());

    // TODO: find formula for honor reward based on player's level!

    // now fixed only for level 70 players:
    if (((Player*)unitTarget)->getLevel() == 70)
        ((Player*)unitTarget)->RewardHonor(NULL, 1, this->damage);
}

void Spell::EffectTradeSkill(uint32 /*i*/)
{
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    // uint32 skillid =  m_spellInfo->EffectMiscValue[i];
    // uint16 skillmax = ((Player*)unitTarget)->(skillid);
    // ((Player*)unitTarget)->SetSkill(skillid,skillval?skillval:1,skillmax+75);
}

void Spell::EffectEnchantItemPerm(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    if (!itemTarget)
        return;

    Player* p_caster = (Player*)m_caster;

    p_caster->UpdateCraftSkill(m_spellInfo->Id);

    if (m_spellInfo->EffectMiscValue[i])
    {
        uint32 enchant_id = m_spellInfo->EffectMiscValue[i];

        SpellItemEnchantmentEntry const *pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if(!pEnchant)
            return;

        // item can be in trade slot and have owner diff. from caster
        Player* item_owner = itemTarget->GetOwner();
        if(!item_owner)
            return;

        if(item_owner!=p_caster && p_caster->GetSession()->GetSecurity() > SEC_PLAYER && sWorld.getConfig(CONFIG_GM_LOG_TRADE) )
            sLog.outCommand("GM %s (Account: %u) enchanting(perm): %s (Entry: %d) for player: %s (Account: %u)",
                p_caster->GetName(),p_caster->GetSession()->GetAccountId(),
                itemTarget->GetProto()->Name1,itemTarget->GetEntry(),
                item_owner->GetName(),item_owner->GetSession()->GetAccountId());

        // remove old enchanting before applying new if equipped
        item_owner->ApplyEnchantment(itemTarget,PERM_ENCHANTMENT_SLOT,false);

        itemTarget->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchant_id, 0, 0);

        // add new enchanting if equipped
        item_owner->ApplyEnchantment(itemTarget,PERM_ENCHANTMENT_SLOT,true);
    }
}

void Spell::EffectEnchantItemTmp(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;

    if(!itemTarget)
        return;

    uint32 enchant_id = m_spellInfo->EffectMiscValue[i];

    // Shaman Rockbiter Weapon
    if(i==0 && m_spellInfo->Effect[1]==SPELL_EFFECT_DUMMY)
    {
        int32 enchnting_damage = m_currentBasePoints[1]+1;

        // enchanting id selected by calculated damage-per-sec stored in Effect[1] base value
        // with already applied percent bonus from Elemental Weapons talent
        // Note: damage calculated (correctly) with rounding int32(float(v)) but
        // RW enchantments applied damage int32(float(v)+0.5), this create  0..1 difference sometime
        switch(enchnting_damage)
        {
            // Rank 1
            case  2: enchant_id =   29; break;              //  0% [ 7% ==  2, 14% == 2, 20% == 2]
            // Rank 2
            case  4: enchant_id =    6; break;              //  0% [ 7% ==  4, 14% == 4]
            case  5: enchant_id = 3025; break;              // 20%
            // Rank 3
            case  6: enchant_id =    1; break;              //  0% [ 7% ==  6, 14% == 6]
            case  7: enchant_id = 3027; break;              // 20%
            // Rank 4
            case  9: enchant_id = 3032; break;              //  0% [ 7% ==  6]
            case 10: enchant_id =  503; break;              // 14%
            case 11: enchant_id = 3031; break;              // 20%
            // Rank 5
            case 15: enchant_id = 3035; break;              // 0%
            case 16: enchant_id = 1663; break;              // 7%
            case 17: enchant_id = 3033; break;              // 14%
            case 18: enchant_id = 3034; break;              // 20%
            // Rank 6
            case 28: enchant_id = 3038; break;              // 0%
            case 29: enchant_id =  683; break;              // 7%
            case 31: enchant_id = 3036; break;              // 14%
            case 33: enchant_id = 3037; break;              // 20%
            // Rank 7
            case 40: enchant_id = 3041; break;              // 0%
            case 42: enchant_id = 1664; break;              // 7%
            case 45: enchant_id = 3039; break;              // 14%
            case 48: enchant_id = 3040; break;              // 20%
            // Rank 8
            case 49: enchant_id = 3044; break;              // 0%
            case 52: enchant_id = 2632; break;              // 7%
            case 55: enchant_id = 3042; break;              // 14%
            case 58: enchant_id = 3043; break;              // 20%
            // Rank 9
            case 62: enchant_id = 2633; break;              // 0%
            case 66: enchant_id = 3018; break;              // 7%
            case 70: enchant_id = 3019; break;              // 14%
            case 74: enchant_id = 3020; break;              // 20%
            default:
                sLog.outError("Spell::EffectEnchantItemTmp: Damage %u not handled in S'RW",enchnting_damage);
                return;
        }
    }

    if (!enchant_id)
    {
        sLog.outError("Spell %u Effect %u (SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY) have 0 as enchanting id",m_spellInfo->Id,i);
        return;
    }

    SpellItemEnchantmentEntry const *pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
    if(!pEnchant)
    {
        sLog.outError("Spell %u Effect %u (SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY) have not existed enchanting id %u ",m_spellInfo->Id,i,enchant_id);
        return;
    }

    // select enchantment duration
    uint32 duration;

    // rogue family enchantments exception by duration
    if(m_spellInfo->Id==38615)
        duration = 1800;                                    // 30 mins
    // other rogue family enchantments always 1 hour (some have spell damage=0, but some have wrong data in EffBasePoints)
    else if(m_spellInfo->SpellFamilyName==SPELLFAMILY_ROGUE)
        duration = 3600;                                    // 1 hour
    // shaman family enchantments
    else if(m_spellInfo->SpellFamilyName==SPELLFAMILY_SHAMAN)
        duration = 1800;                                    // 30 mins
    // other cases with this SpellVisual already selected
    else if(m_spellInfo->SpellVisual==215)
        duration = 1800;                                    // 30 mins
    // some fishing pole bonuses
    else if(m_spellInfo->SpellVisual==563)
        duration = 600;                                     // 10 mins
    // shaman rockbiter enchantments
    else if(m_spellInfo->SpellVisual==0)
        duration = 1800;                                    // 30 mins
    else if(m_spellInfo->Id==29702)
        duration = 300;                                     // 5 mins
    else if(m_spellInfo->Id==37360)
        duration = 300;                                     // 5 mins
    // default case
    else
        duration = 3600;                                    // 1 hour

    // item can be in trade slot and have owner diff. from caster
    Player* item_owner = itemTarget->GetOwner();
    if(!item_owner)
        return;

    if(item_owner!=p_caster && p_caster->GetSession()->GetSecurity() > SEC_PLAYER && sWorld.getConfig(CONFIG_GM_LOG_TRADE) )
        sLog.outCommand("GM %s (Account: %u) enchanting(temp): %s (Entry: %d) for player: %s (Account: %u)",
            p_caster->GetName(),p_caster->GetSession()->GetAccountId(),
            itemTarget->GetProto()->Name1,itemTarget->GetEntry(),
            item_owner->GetName(),item_owner->GetSession()->GetAccountId());

    // remove old enchanting before applying new if equipped
    item_owner->ApplyEnchantment(itemTarget,TEMP_ENCHANTMENT_SLOT,false);

    itemTarget->SetEnchantment(TEMP_ENCHANTMENT_SLOT, enchant_id, duration*1000, 0);

    // add new enchanting if equipped
    item_owner->ApplyEnchantment(itemTarget,TEMP_ENCHANTMENT_SLOT,true);
}

void Spell::EffectTameCreature(uint32 /*i*/)
{
    if(m_caster->GetPetGUID())
        return;

    if(!unitTarget)
        return;

    if(unitTarget->GetTypeId() == TYPEID_PLAYER)
        return;

    Creature* creatureTarget = (Creature*)unitTarget;

    if(creatureTarget->isPet())
        return;

    if(m_caster->getClass() == CLASS_HUNTER)
    {
        // cast finish successfully
        //SendChannelUpdate(0);
        finish();

        Pet* pet = new Pet(m_caster, HUNTER_PET);

        if(!pet->CreateBaseAtCreature(creatureTarget))
        {
            delete pet;
            return;
        }

        creatureTarget->setDeathState(JUST_DIED);
        creatureTarget->RemoveCorpse();
        creatureTarget->SetHealth(0);                       // just for nice GM-mode view

        pet->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, m_caster->GetGUID());
        pet->SetUInt64Value(UNIT_FIELD_CREATEDBY, m_caster->GetGUID());
        pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,m_caster->getFaction());
        pet->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);

        if(!pet->InitStatsForLevel(creatureTarget->getLevel()))
        {
            sLog.outError("ERROR: InitStatsForLevel() in EffectTameCreature failed! Pet deleted.");
            delete pet;
            return;
        }

        // prepare visual effect for levelup
        pet->SetUInt32Value(UNIT_FIELD_LEVEL,creatureTarget->getLevel()-1);

        pet->GetCharmInfo()->SetPetNumber(objmgr.GeneratePetNumber(), true);
                                                            // this enables pet details window (Shift+P)
        pet->AIM_Initialize();
        pet->InitPetCreateSpells();
        pet->SetHealth(pet->GetMaxHealth());

        MapManager::Instance().GetMap(pet->GetMapId(), pet)->Add((Creature*)pet);

        // visual effect for levelup
        pet->SetUInt32Value(UNIT_FIELD_LEVEL,creatureTarget->getLevel());

        if(m_caster->GetTypeId() == TYPEID_PLAYER)
        {
            m_caster->SetPet(pet);
            pet->SavePetToDB(PET_SAVE_AS_CURRENT);
            ((Player*)m_caster)->PetSpellInitialize();
        }
    }
}

void Spell::EffectSummonPet(uint32 i)
{
    float px, py, pz;
    m_caster->GetClosePoint(px, py, pz);

    uint32 petentry = m_spellInfo->EffectMiscValue[i];

    Pet *OldSummon = m_caster->GetPet();

    // if pet requested type already exist
    if( OldSummon )
    {
        if(petentry == 0 || OldSummon->GetCreatureInfo()->Entry == petentry)
        {
            // pet in corpse state can't be summoned
            if( OldSummon->isDead() )
                return;

            MapManager::Instance().GetMap(OldSummon->GetMapId(), OldSummon)->Remove((Creature*)OldSummon,false);
            OldSummon->SetMapId(m_caster->GetMapId());
            OldSummon->Relocate(px, py, pz, OldSummon->GetOrientation());
            MapManager::Instance().GetMap(m_caster->GetMapId(), m_caster)->Add((Creature*)OldSummon);

            if(m_caster->GetTypeId() == TYPEID_PLAYER && OldSummon->isControlled() )
            {
                ((Player*)m_caster)->PetSpellInitialize();
            }
            return;
        }

        if(m_caster->GetTypeId() == TYPEID_PLAYER)
            ((Player*)m_caster)->RemovePet(OldSummon,(OldSummon->getPetType()==HUNTER_PET ? PET_SAVE_AS_DELETED : PET_SAVE_NOT_IN_SLOT),false);
        else
            return;
    }

    Pet* NewSummon = new Pet(m_caster);

    // petentry==0 for hunter "call pet" (current pet summoned if any)
    if(NewSummon->LoadPetFromDB(m_caster,petentry))
    {
        if(NewSummon->getPetType()==SUMMON_PET)
        {
            // Remove Demonic Sacrifice auras (known pet)
            Unit::AuraList const& auraClassScripts = m_caster->GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
            for(Unit::AuraList::const_iterator itr = auraClassScripts.begin();itr!=auraClassScripts.end();)
            {
                if((*itr)->GetModifier()->m_miscvalue==2228)
                {
                    m_caster->RemoveAurasDueToSpell((*itr)->GetId());
                    itr = auraClassScripts.begin();
                }
                else
                    ++itr;
            }
        }

        return;
    }

    // not error in case fail hunter call pet
    if(!petentry)
    {
        delete NewSummon;
        return;
    }

    CreatureInfo const* cInfo = sCreatureStorage.LookupEntry<CreatureInfo>(petentry);

    if(!cInfo)
    {
        sLog.outError("EffectSummonPet: creature entry %u not found.",petentry);
        delete NewSummon;
        return;
    }

    uint32 pet_number = objmgr.GeneratePetNumber();
    if( NewSummon->Create(objmgr.GenerateLowGuid(HIGHGUID_PET),  m_caster->GetMapId(), px, py, pz+1, m_caster->GetOrientation(), petentry, pet_number))
    {
        uint32 petlevel = m_caster->getLevel();
        NewSummon->setPetType(SUMMON_PET);

        uint32 faction = m_caster->getFaction();
        if(m_caster->GetTypeId() == TYPEID_UNIT && ((Creature*)m_caster)->isTotem())
        {
            Unit* owner = ((Totem*)m_caster)->GetOwner();
            if(owner) faction = owner->getFaction();
            NewSummon->GetCharmInfo()->SetReactState(REACT_AGGRESSIVE);
        }

        NewSummon->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, m_caster->GetGUID());
        NewSummon->SetUInt64Value(UNIT_FIELD_CREATEDBY, m_caster->GetGUID());
        NewSummon->SetUInt32Value(UNIT_NPC_FLAGS , 0);
        NewSummon->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, faction);
        NewSummon->SetUInt32Value(UNIT_FIELD_BYTES_0,2048);
        NewSummon->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
        NewSummon->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP,time(NULL));
        NewSummon->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE,0);
        NewSummon->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP,1000);
        NewSummon->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);

        NewSummon->GetCharmInfo()->SetPetNumber(pet_number, true);
                                                            // this enables pet details window (Shift+P)

        // this enables popup window (pet dismiss, cancel), hunter pet additional flags set later
        NewSummon->SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_UNKNOWN1);

        NewSummon->InitStatsForLevel( petlevel);
        NewSummon->InitPetCreateSpells();

        if(NewSummon->getPetType()==SUMMON_PET)
        {
            // Remove Demonic Sacrifice auras (new pet)
            Unit::AuraList const& auraClassScripts = m_caster->GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
            for(Unit::AuraList::const_iterator itr = auraClassScripts.begin();itr!=auraClassScripts.end();)
            {
                if((*itr)->GetModifier()->m_miscvalue==2228)
                {
                    m_caster->RemoveAurasDueToSpell((*itr)->GetId());
                    itr = auraClassScripts.begin();
                }
                else
                    ++itr;
            }

            // generate new name for summon pet
            std::string new_name=objmgr.GeneratePetName(petentry);
            if(!new_name.empty())
                NewSummon->SetName(new_name);
        }
        else if(NewSummon->getPetType()==HUNTER_PET)
            NewSummon->SetByteValue(UNIT_FIELD_BYTES_2, 2, 0x02);

        NewSummon->AIM_Initialize();
        NewSummon->SetHealth(NewSummon->GetMaxHealth());
        NewSummon->SetPower(POWER_MANA, NewSummon->GetMaxPower(POWER_MANA));

        MapManager::Instance().GetMap(NewSummon->GetMapId(), NewSummon)->Add((Creature*)NewSummon);

        m_caster->SetPet(NewSummon);
        sLog.outDebug("New Pet has guid %u", NewSummon->GetGUIDLow());

        if(m_caster->GetTypeId() == TYPEID_PLAYER)
        {
            NewSummon->SavePetToDB(PET_SAVE_AS_CURRENT);
            ((Player*)m_caster)->PetSpellInitialize();
        }
    }
    else
        delete NewSummon;
}

void Spell::EffectLearnPetSpell(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)m_caster;

    Pet *pet = _player->GetPet();
    if(!pet)
        return;
    if(!pet->isAlive())
        return;

    SpellEntry const *learn_spellproto = sSpellStore.LookupEntry(m_spellInfo->EffectTriggerSpell[i]);
    if(!learn_spellproto)
        return;

    pet->SetTP(pet->m_TrainingPoints - pet->GetTPForSpell(learn_spellproto->Id));
    pet->learnSpell(learn_spellproto->Id);

    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
    _player->PetSpellInitialize();
}

void Spell::EffectTaunt(uint32 /*i*/)
{
    // this effect use before aura Taunt apply for prevent taunt already attacking target
    // for spell as marked "non effective at already attacking target"
    if(unitTarget && unitTarget->GetTypeId() != TYPEID_PLAYER)
    {
        if(unitTarget->getVictim()==m_caster)
        {
            SendCastResult(SPELL_FAILED_DONT_REPORT);
            return;
        }
    }

    // Also use this effect to set the taunter's threat to the taunted creature's highest value
    if(unitTarget->CanHaveThreatList() && unitTarget->getThreatManager().getCurrentVictim())
        unitTarget->getThreatManager().addThreat(m_caster,unitTarget->getThreatManager().getCurrentVictim()->getThreat());
}

void Spell::EffectWeaponDmg(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    uint32 wp[4] = { SPELL_EFFECT_WEAPON_DAMAGE, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE, SPELL_EFFECT_NORMALIZED_WEAPON_DMG, SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL };

    // multiple weap dmg effect workaround
    // execute only the first weapon damage
    // and handle all effects at once
    uint8 j,k;
    int32 bonus = 0;

    for (j = 0; j < 3; j++)
    {
        for (k = 0; k < 4; k++)
            if (m_spellInfo->Effect[j] == wp[k])
                break;
        if (k != 4)
        {
            if (j < i)
                return;
            if (m_spellInfo->Effect[j] != SPELL_EFFECT_WEAPON_PERCENT_DAMAGE)
                bonus += CalculateDamage(j,unitTarget);
        }
    }

    // Devastate bonus and sunder armor refresh
    if(m_spellInfo->SpellVisual == 671 && m_spellInfo->SpellIconID == 1508)
    {
        int32 sp_bonus = 0;
        for(int x=0;x<3;++x)
        {
            if(m_spellInfo->Effect[x]==SPELL_EFFECT_NORMALIZED_WEAPON_DMG)
            {
                sp_bonus=CalculateDamage(x,unitTarget);
                break;
            }
        }

        uint32 sunder_stacks = 0;
        Unit::AuraList const& list = unitTarget->GetAurasByType(SPELL_AURA_MOD_RESISTANCE);
        for(Unit::AuraList::const_iterator itr=list.begin();itr!=list.end();++itr)
        {
            SpellEntry const *proto = (*itr)->GetSpellProto();
            if(proto->SpellVisual == 406 && proto->SpellIconID == 565)
            {
                int32 duration = GetSpellDuration(proto);
                (*itr)->SetAuraDuration(duration);
                (*itr)->UpdateAuraDuration();
                ++sunder_stacks;
            }
        }

        bonus+= sp_bonus*sunder_stacks;
    }
    // Note: bonus can be negative

    uint32 hitInfo = 0;
    uint32 nohitMask = HITINFO_ABSORB | HITINFO_RESIST | HITINFO_MISS;
    VictimState victimState = VICTIMSTATE_NORMAL;
    uint32 blocked_dmg = 0;
    uint32 absorbed_dmg = 0;
    uint32 resisted_dmg = 0;
    CleanDamage cleanDamage =  CleanDamage(0, BASE_ATTACK, MELEE_HIT_NORMAL );
    bool criticalhit = false;

    // calculate damage percent modifier
    float damagePercentMod = 1;
    for (j = 0; j < 3; j++)
        if (m_spellInfo->Effect[j] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE)
            damagePercentMod *= (float(CalculateDamage(j,unitTarget)) / 100);

    // set base eff_damage, total normal hit damage after DoAttackDamage call will be bonus + weapon
    // if miss/parry, no eff=0 automatically by func DoAttackDamage
    // if crit eff = (bonus + weapon) * 2
    // In a word, bonus + weapon will be calculated together in cases of miss, armor reduce, crit, etc.
    bool normalized = false;
    for (j = 0; j <3;j++)
        if (m_spellInfo->Effect[j] ==  SPELL_EFFECT_NORMALIZED_WEAPON_DMG)
            normalized = true;
 
    bonus += m_caster->CalculateDamage(m_attackType, normalized);

    switch(m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_WARRIOR:
        {
            // Whirlwind, single only spell with 2 weapon white damage apply if have
            if(m_caster->GetTypeId()==TYPEID_PLAYER && (m_spellInfo->SpellFamilyFlags & 0x00000400000000LL))
            {
                Item* item = ((Player*)m_caster)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (item && item->GetProto()->Class == ITEM_CLASS_WEAPON && !item->IsBroken() && ((Player*)m_caster)->IsUseEquipedWeapon(false))
                    bonus += m_caster->CalculateDamage (OFF_ATTACK, normalized);
            }
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            // Mutilate (for each hand)
            if(m_spellInfo->SpellFamilyFlags & 0x600000000LL)
            {
                Unit::AuraMap const& auras = unitTarget->GetAuras();
                for(Unit::AuraMap::const_iterator itr = auras.begin(); itr!=auras.end(); ++itr)
                {
                    if(itr->second->GetSpellProto()->Dispel == DISPEL_POISON)
                    {
                        // 150% damage
                        damagePercentMod *= 1.5f;
                        break;
                    }
                }
            }
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Seal of Command - receive benefit from Spell Damage and Healing
            if(m_spellInfo->SpellFamilyFlags & 0x00000002000000LL)
            {
                bonus += int32(0.20f*m_caster->SpellBaseDamageBonus(GetSpellSchoolMask(m_spellInfo)));
                bonus += int32(0.29f*m_caster->SpellBaseDamageBonusForVictim(GetSpellSchoolMask(m_spellInfo), unitTarget));
            }
            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            // Skyshatter Harness item set bonus
            // Stormstrike
            if(m_spellInfo->SpellFamilyFlags & 0x001000000000LL)
            {
                Unit::AuraList const& m_OverrideClassScript = m_caster->GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
                for(Unit::AuraList::const_iterator i = m_OverrideClassScript.begin(); i != m_OverrideClassScript.end(); ++i)
                {
                    // Stormstrike AP Buff
                    if ( (*i)->GetModifier()->m_miscvalue == 5634 )
                    {
                        m_caster->CastSpell(m_caster,38430,true,NULL,*i);
                        break;
                    }
                }
            }
        }
    }

    // percent mod applied
    bonus = int32(bonus *damagePercentMod);

    // prevent negative damage
    uint32 eff_damage = uint32(bonus > 0 ? bonus : 0);

    m_caster->DoAttackDamage(unitTarget, &eff_damage, &cleanDamage, &blocked_dmg, m_spellSchoolMask, &hitInfo, &victimState, &absorbed_dmg, &resisted_dmg, m_attackType, m_spellInfo, m_IsTriggeredSpell);

    if ((hitInfo & nohitMask) && m_attackType != RANGED_ATTACK)  // not send ranged miss/etc
        m_caster->SendAttackStateUpdate(hitInfo & nohitMask, unitTarget, 1, m_spellSchoolMask, eff_damage, absorbed_dmg, resisted_dmg, VICTIMSTATE_NORMAL, blocked_dmg);

    if(hitInfo & HITINFO_CRITICALHIT)
        criticalhit = true;

    m_caster->SendSpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, eff_damage, m_spellSchoolMask, absorbed_dmg, resisted_dmg, false, blocked_dmg, criticalhit);

    if (eff_damage > (absorbed_dmg + resisted_dmg + blocked_dmg))
    {
        eff_damage -= (absorbed_dmg + resisted_dmg + blocked_dmg);
    }
    else
    {
        cleanDamage.damage += eff_damage;
        eff_damage = 0;
    }

    // SPELL_SCHOOL_NORMAL use for weapon-like threat and rage calculation
    m_caster->DealDamage(unitTarget, eff_damage, &cleanDamage, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, true);

    // Hemorrhage
    if(m_spellInfo->SpellFamilyName==SPELLFAMILY_ROGUE && (m_spellInfo->SpellFamilyFlags & 0x2000000))
    {
        if(m_caster->GetTypeId()==TYPEID_PLAYER)
            ((Player*)m_caster)->AddComboPoints(unitTarget, 1);
    }

    // take ammo
    if(m_attackType == RANGED_ATTACK && m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        Item *pItem = ((Player*)m_caster)->GetItemByPos( INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED );
        if(!pItem  || pItem->IsBroken())
            return;

        if( pItem->GetProto()->InventoryType == INVTYPE_THROWN )
        {
            if(pItem->GetMaxStackCount()==1)
            {
                // decrease durability for non-stackable throw weapon
                ((Player*)m_caster)->DurabilityPointsLoss(EQUIPMENT_SLOT_RANGED,1);
            }
            else
            {
                // decrease items amount for stackable throw weapon
                uint32 count = 1;
                ((Player*)m_caster)->DestroyItemCount( pItem, count, true);
            }
        }
        else if(uint32 ammo = ((Player*)m_caster)->GetUInt32Value(PLAYER_AMMO_ID))
            ((Player*)m_caster)->DestroyItemCount(ammo, 1, true);
        // wand not have ammo
    }
}

void Spell::EffectThreat(uint32 /*i*/)
{
    if(!unitTarget || !unitTarget->isAlive() || !m_caster->isAlive())
        return;

    if(!unitTarget->CanHaveThreatList())
        return;

    unitTarget->AddThreat(m_caster, float(damage));
}

void Spell::EffectHealMaxHealth(uint32 /*i*/)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    uint32 heal = m_caster->GetMaxHealth();

    if(m_spellInfo->SpellVisual == 132)                     // drain all caster's mana
        m_caster->SetPower(POWER_MANA, 0);

    int32 gain = unitTarget->ModifyHealth(heal);
    unitTarget->getHostilRefManager().threatAssist(m_caster, float(gain) * 0.5f, m_spellInfo);

    m_caster->SendHealSpellLog(unitTarget, m_spellInfo->Id, heal);
}

void Spell::EffectInterruptCast(uint32 /*i*/)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    // TODO: not all spells that used this effect apply cooldown at school spells
    // also exist case: apply cooldown to interrupted cast only and to all spells
    for (uint32 i = CURRENT_FIRST_NON_MELEE_SPELL; i < CURRENT_MAX_SPELL; i++)
    {
        if (unitTarget->m_currentSpells[i])
        {
            // check if we can interrupt spell
            if ( unitTarget->m_currentSpells[i]->m_spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_INTURRUPT )
            {
                unitTarget->ProhibitSpellScholl(GetSpellSchoolMask(unitTarget->m_currentSpells[i]->m_spellInfo), GetSpellDuration(m_spellInfo));
                unitTarget->InterruptSpell(i,false);
            }
        }
    }
}

void Spell::EffectSummonObjectWild(uint32 i)
{
    GameObject* pGameObj = new GameObject(m_caster);

    uint32 gameobject_id = m_spellInfo->EffectMiscValue[i];

    WorldObject* target = focusObject;
    if( !target )
        target = m_caster;

    // before caster
    float x,y,z;
    m_caster->GetClosePoint(x,y,z);

    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), gameobject_id, target->GetMapId(),
        x, y, z, target->GetOrientation(), 0, 0, 0, 0, 100, 1))
    {
        delete pGameObj;
        return;
    }

    int32 duration = GetSpellDuration(m_spellInfo);
    pGameObj->SetRespawnTime(duration > 0 ? duration/1000 : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);

    if(pGameObj->GetGoType() != GAMEOBJECT_TYPE_FLAGDROP)   // make dropped flag clickable for other players (not set owner guid (created by) for this)...
        m_caster->AddGameObject(pGameObj);
    MapManager::Instance().GetMap(pGameObj->GetMapId(), pGameObj)->Add(pGameObj);

    if(pGameObj->GetMapId() == 489 && pGameObj->GetGoType() == GAMEOBJECT_TYPE_FLAGDROP)  //WS
    {
        if(m_caster->GetTypeId() == TYPEID_PLAYER)
        {
            Player *pl = (Player*)m_caster;
            BattleGround* bg = ((Player *)m_caster)->GetBattleGround();
            if(bg && bg->GetTypeID()==BATTLEGROUND_WS && bg->GetStatus() == STATUS_IN_PROGRESS)
            {
                 uint32 team = ALLIANCE;

                 if(pl->GetTeam() == team)
                     team = HORDE;

                ((BattleGroundWS*)bg)->SetDroppedFlagGUID(pGameObj->GetGUID(),team);
            }
        }
    }

    if(pGameObj->GetMapId() == 566 && pGameObj->GetGoType() == GAMEOBJECT_TYPE_FLAGDROP)  //EY
    {
        if(m_caster->GetTypeId() == TYPEID_PLAYER)
        {
            Player *pl = (Player*)m_caster;
            BattleGround* bg = ((Player *)m_caster)->GetBattleGround();
            if(bg && bg->GetTypeID()==BATTLEGROUND_EY && bg->GetStatus() == STATUS_IN_PROGRESS)
            {
                ((BattleGroundEY*)bg)->SetDroppedFlagGUID(pGameObj->GetGUID());
            }
        }
    }

    if(uint32 linkedEntry = pGameObj->GetLinkedGameObjectEntry())
    {
        GameObject* linkedGO = new GameObject(m_caster);
        if(linkedGO->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), linkedEntry, target->GetMapId(),
            x, y, z, target->GetOrientation(), 0, 0, 0, 0, 100, 1))
        {
            linkedGO->SetRespawnTime(duration > 0 ? duration/1000 : 0);
            linkedGO->SetSpellId(m_spellInfo->Id);

            m_caster->AddGameObject(linkedGO);
            MapManager::Instance().GetMap(linkedGO->GetMapId(), linkedGO)->Add(linkedGO);
        }
        else
        {
            delete linkedGO;
            linkedGO = NULL;
            return;
        }
    }
}

void Spell::EffectScriptEffect(uint32 i)
{
    // TODO: we must implement hunter pet summon at login there (spell 6962)

    // by spell id
    switch(m_spellInfo->Id)
    {
        // Keep in top, because core use it for npc aggro other npc
        case SPELL_ID_AGGRO:
        {
            if( !m_caster || m_caster->GetTypeId() != TYPEID_UNIT || !m_caster->getVictim() )
                return;

            float radius = sWorld.getConfig(CONFIG_CREATURE_FAMILY_ASSISTEMCE_RADIUS);
            if(radius > 0)
            {
                std::list<Creature*> assistList;

                {
                    CellPair p(MaNGOS::ComputeCellPair(m_caster->GetPositionX(), m_caster->GetPositionY()));
                    Cell cell(p);
                    cell.data.Part.reserved = ALL_DISTRICT;
                    cell.SetNoCreate();

                    MaNGOS::AnyAssistCreatureInRangeCheck u_check(m_caster, m_caster->getVictim(), radius);
                    MaNGOS::CreatureListSearcher<MaNGOS::AnyAssistCreatureInRangeCheck> searcher(assistList, u_check);

                    TypeContainerVisitor<MaNGOS::CreatureListSearcher<MaNGOS::AnyAssistCreatureInRangeCheck>, GridTypeMapContainer >  grid_creature_searcher(searcher);

                    CellLock<GridReadGuard> cell_lock(cell, p);
                    cell_lock->Visit(cell_lock, grid_creature_searcher, *MapManager::Instance().GetMap(m_caster->GetMapId(), m_caster));
                }

                for(std::list<Creature*>::iterator iter = assistList.begin(); iter != assistList.end(); ++iter)
                {
                    ((Creature*)m_caster)->SetNoCallAssistence(true);
                    (*iter)->SetNoCallAssistence(true);
                    if((*iter)->AI())
                        (*iter)->AI()->AttackStart(m_caster->getVictim());
                }
            }

            return;
        }

        // Bending Shinbone
        case 8856:
        {
            if(!itemTarget && m_caster->GetTypeId()!=TYPEID_PLAYER)
                return;

            uint32 spell_id = 0;
            switch(urand(1,5))
            {
                case 1:  spell_id = 8854; break;
                default: spell_id = 8855; break;
            }

            m_caster->CastSpell(m_caster,spell_id,true,NULL);
            return;
        }

        // Healthstone creating spells
        case  6201:
        case  6202:
        case  5699:
        case 11729:
        case 11730:
        case 27230:
        {
            uint32 itemtype;
            uint32 rank = 0;
            Unit::AuraList const& mDummyAuras = unitTarget->GetAurasByType(SPELL_AURA_DUMMY);
            for(Unit::AuraList::const_iterator i = mDummyAuras.begin();i != mDummyAuras.end(); ++i)
            {
                if((*i)->GetId() == 18692)
                {
                    rank = 1;
                    break;
                }
                else if((*i)->GetId() == 18693)
                {
                    rank = 2;
                    break;
                }
            }

            static uint32 const itypes[6][3] = {
                { 5512,19004,19005},                        // Minor Healthstone
                { 5511,19006,19007},                        // Lesser Healthstone
                { 5509,19008,19009},                        // Healthstone
                { 5510,19010,19011},                        // Greater Healthstone
                { 9421,19012,19013},                        // Major Healthstone
                {22103,22104,22105}                         // Master Healthstone
            };

            switch(m_spellInfo->Id)
            {
                case  6201: itemtype=itypes[0][rank];break; // Minor Healthstone
                case  6202: itemtype=itypes[1][rank];break; // Lesser Healthstone
                case  5699: itemtype=itypes[2][rank];break; // Healthstone
                case 11729: itemtype=itypes[3][rank];break; // Greater Healthstone
                case 11730: itemtype=itypes[4][rank];break; // Major Healthstone
                case 27230: itemtype=itypes[5][rank];break; // Master Healthstone
                default:
                    return;
            }
            DoCreateItem( i, itemtype );
            return;
        }
        // Brittle Armor - need remove one 24575 Brittle Armor aura 
        case 24590:
            unitTarget->RemoveSingleAuraFromStack(24575, 0);
            unitTarget->RemoveSingleAuraFromStack(24575, 1);
            return;
        // Mercurial Shield - need remove one 26464 Mercurial Shield aura 
        case 26465:
            unitTarget->RemoveSingleAuraFromStack(26464, 0);
            return;
        // Orb teleport spells
        case 25140:
        case 25143:
        case 25650:
        case 25652:
        case 29128:
        case 29129:
        case 35376:
        case 35727:
        {
            if(!unitTarget)
                return;

            uint32 spellid;
            switch(m_spellInfo->Id)
            {
                case 25140: spellid =  32571; break;
                case 25143: spellid =  32572; break;
                case 25650: spellid =  30140; break;
                case 25652: spellid =  30141; break;
                case 29128: spellid =  32568; break;
                case 29129: spellid =  32569; break;
                case 35376: spellid =  25649; break;
                case 35727: spellid =  35730; break;
                default:
                    return;
            }

            unitTarget->CastSpell(unitTarget,spellid,false);
            return;
        }

        // Shadow Flame (All script effects, not just end ones to prevent player from dodging the last triggered spell)
        case 22539:
        case 22972:
        case 22975:
        case 22976:
        case 22977:
        case 22978:
        case 22979:
        case 22980:
        case 22981:
        case 22982:
        case 22983:
        case 22984:
        case 22985:
        {
            if(!unitTarget || !unitTarget->isAlive())
                return;

            // Onyxia Scale Cloak
            if(unitTarget->GetDummyAura(22683))
                return;

            // Shadow Flame
            m_caster->CastSpell(unitTarget, 22682, true);
            return;
        }
        break;

        // Summon Black Qiraji Battle Tank
        case 26656:
        {
            if(!unitTarget)
                return;

            // Prevent stacking of mounts
            unitTarget->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);

            // Two separate mounts depending on area id (allows use both in and out of specific instance)
            if (unitTarget->GetAreaId() == 3428)
                unitTarget->CastSpell(unitTarget, 25863, false);
            else
                unitTarget->CastSpell(unitTarget, 26655, false);
            break;
        }
        // Piccolo of the Flaming Fire
        case 17512:
        {
            if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
                return;
            unitTarget->HandleEmoteCommand(EMOTE_STATE_DANCE);
            break;
        }

        // Mirren's Drinking Hat
        case 29830:
        {
            uint32 item = 0;
            switch ( urand(1,6) )
            {
                case 1: case 2: case 3: item = 23584; break;// Loch Modan Lager
                case 4: case 5:         item = 23585; break;// Stouthammer Lite
                case 6:                 item = 23586; break;// Aerie Peak Pale Ale
            }
            if (item)
                DoCreateItem(i,item);
            break;
        }
        // Improved Sprint
        case 30918:
        {
            // Removes snares and roots.
            uint32 mechanic = (1<<MECHANIC_ROOT) | (1<<MECHANIC_SNARE);
            Unit::AuraMap& Auras = unitTarget->GetAuras();
            for(Unit::AuraMap::iterator iter = Auras.begin(), next; iter != Auras.end(); iter = next)
            {
                next = iter;
                ++next;
                Aura *aur = iter->second;
                if (!aur->IsPositive())             //only remove negative spells
                {
                    // check for mechanic mask
                    if(GetSpellMechanicMask(aur->GetSpellProto(), aur->GetEffIndex()) & mechanic)
                    {
                        unitTarget->RemoveAurasDueToSpell(aur->GetId());
                        if(Auras.empty())
                            break;
                        else
                            next = Auras.begin();
                    }
                }
            }
            break;
        }
        // Flame Crash
        case 41126:
        {
            if(!unitTarget)
                return;

           unitTarget->CastSpell(unitTarget, 41131, true);
           break;
        }

        // Goblin Weather Machine
        case 46203:
        {
            if(!unitTarget)
                return;

            uint32 spellId;
            switch(rand()%4)
            {
                case 0:
                    spellId=46740;
                    break;
                case 1:
                    spellId=46739;
                    break;
                case 2:
                    spellId=46738;
                    break;
                case 3:
                    spellId=46736;
                    break;
            }
            unitTarget->CastSpell(unitTarget, spellId, true);
            break;
        }
    }

    if( m_spellInfo->SpellFamilyName == SPELLFAMILY_PALADIN )
    {
        switch(m_spellInfo->SpellFamilyFlags)
        {
            // Judgement
            case 0x800000:
            {
                if(!unitTarget || !unitTarget->isAlive())
                    return;
                uint32 spellId2 = 0;

                // all seals have aura dummy
                Unit::AuraList const& m_dummyAuras = m_caster->GetAurasByType(SPELL_AURA_DUMMY);
                for(Unit::AuraList::const_iterator itr = m_dummyAuras.begin(); itr != m_dummyAuras.end(); ++itr)
                {
                    SpellEntry const *spellInfo = (*itr)->GetSpellProto();

                    // search seal (all seals have judgement's aura dummy spell id in 2 effect
                    if ( !spellInfo || !IsSealSpell((*itr)->GetSpellProto()) || (*itr)->GetEffIndex() != 2 )
                        continue;

                    // must be calculated base at raw base points in spell proto, GetModifier()->m_value for S.Righteousness modified by SPELLMOD_DAMAGE
                    spellId2 = (*itr)->GetSpellProto()->EffectBasePoints[2]+1;

                    if(spellId2 <= 1)
                        continue;

                    // found, remove seal
                    m_caster->RemoveAurasDueToSpell((*itr)->GetId());

                    // Sanctified Judgement
                    Unit::AuraList const& m_auras = m_caster->GetAurasByType(SPELL_AURA_DUMMY);
                    for(Unit::AuraList::const_iterator i = m_auras.begin(); i != m_auras.end(); ++i)
                    {
                        if ((*i)->GetSpellProto()->SpellIconID == 205 && (*i)->GetSpellProto()->Attributes == 0x01D0LL)
                        {
                            int32 chance = (*i)->GetModifier()->m_amount;
                            if ( roll_chance_i(chance) )
                            {
                                int32 mana = spellInfo->manaCost;
                                if ( Player* modOwner = m_caster->GetSpellModOwner() )
                                    modOwner->ApplySpellMod(spellInfo->Id, SPELLMOD_COST, mana);
                                mana = int32(mana* 0.8f);
                                m_caster->CastCustomSpell(m_caster,31930,&mana,NULL,NULL,true,NULL,*i);
                            }
                            break;
                        }
                    }

                    break;
                }

                m_caster->CastSpell(unitTarget,spellId2,true);
                return;
            }
        }
    }

    // normal DB scripted effect
    if(!unitTarget)
        return;

    sLog.outDebug("Spell ScriptStart spellid %u in EffectScriptEffect ", m_spellInfo->Id);
    sWorld.ScriptsStart(sSpellScripts, m_spellInfo->Id, m_caster, unitTarget);
}

void Spell::EffectSanctuary(uint32 /*i*/)
{
    if(!unitTarget)
        return;
    //unitTarget->CombatStop();

    unitTarget->CombatStop();
    unitTarget->getHostilRefManager().deleteReferences();   // stop all fighting
    // Vanish allows to remove all threat and cast regular stealth so other spells can be used
    if(m_spellInfo->SpellFamilyName == SPELLFAMILY_ROGUE && (m_spellInfo->SpellFamilyFlags & SPELLFAMILYFLAG_ROGUE_VANISH))
    {
        ((Player *)m_caster)->RemoveSpellsCausingAura(SPELL_AURA_MOD_ROOT);
    }
}

void Spell::EffectAddComboPoints(uint32 /*i*/)
{
    if(!unitTarget)
        return;

    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    if(damage <= 0)
        return;

    ((Player*)m_caster)->AddComboPoints(unitTarget, damage);
}

void Spell::EffectDuel(uint32 i)
{
    if(!m_caster || !unitTarget || m_caster->GetTypeId() != TYPEID_PLAYER || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *caster = (Player*)m_caster;
    Player *target = (Player*)unitTarget;

    // caster or target already have requested duel
    if( caster->duel || target->duel || target->GetSocial()->HasIgnore(caster->GetGUIDLow()) )
        return;

    // Players can only fight a duel with each other outside (=not inside dungeons and not in capital cities)
    // Don't have to check the target's map since you cannot challenge someone across maps
    if ((caster->GetMapId() != 0 && caster->GetMapId() != 1 && caster->GetMapId() != 530) ||
        (GetAreaEntryByAreaID(caster->GetZoneId()) && (GetAreaEntryByAreaID(caster->GetZoneId())->flags & AREA_FLAG_CAPITAL) != 0) ||
        (GetAreaEntryByAreaID(target->GetZoneId()) && (GetAreaEntryByAreaID(target->GetZoneId())->flags & AREA_FLAG_CAPITAL) != 0))
    {
        SendCastResult(SPELL_FAILED_NO_DUELING);            // Dueling isn't allowed here
        return;
    }

    //CREATE DUEL FLAG OBJECT
    GameObject* pGameObj = new GameObject(m_caster);

    uint32 gameobject_id = m_spellInfo->EffectMiscValue[i];

    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), gameobject_id,m_caster->GetMapId(),
        m_caster->GetPositionX()+(unitTarget->GetPositionX()-m_caster->GetPositionX())/2 ,
        m_caster->GetPositionY()+(unitTarget->GetPositionY()-m_caster->GetPositionY())/2 ,
        m_caster->GetPositionZ(),
        m_caster->GetOrientation(), 0, 0, 0, 0, 0, 1))
    {
        delete pGameObj;
        return;
    }

    pGameObj->SetUInt32Value(GAMEOBJECT_FACTION, m_caster->getFaction() );
    pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, m_caster->getLevel()+1 );
    int32 duration = GetSpellDuration(m_spellInfo);
    pGameObj->SetRespawnTime(duration > 0 ? duration/1000 : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);

    m_caster->AddGameObject(pGameObj);
    MapManager::Instance().GetMap(pGameObj->GetMapId(), pGameObj)->Add(pGameObj);
    //END

    // Send request
    WorldPacket data(SMSG_DUEL_REQUESTED, 16);
    data << pGameObj->GetGUID();
    data << caster->GetGUID();
    caster->GetSession()->SendPacket(&data);
    target->GetSession()->SendPacket(&data);

    // create duel-info
    DuelInfo *duel   = new DuelInfo;
    duel->initiator  = caster;
    duel->opponent   = target;
    duel->startTime  = 0;
    duel->startTimer = 0;
    caster->duel     = duel;

    DuelInfo *duel2   = new DuelInfo;
    duel2->initiator  = caster;
    duel2->opponent   = caster;
    duel2->startTime  = 0;
    duel2->startTimer = 0;
    target->duel      = duel2;

    caster->SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());
    target->SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());
}

void Spell::EffectStuck(uint32 /*i*/)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if(!sWorld.getConfig(CONFIG_CAST_UNSTUCK))
        return;

    Player* pTarget = (Player*)unitTarget;

    sLog.outDebug("Spell Effect: Stuck");
    sLog.outDetail("Player %s (guid %u) used auto-unstuck future at map %u (%f, %f, %f)", pTarget->GetName(), pTarget->GetGUIDLow(), m_caster->GetMapId(), m_caster->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ());

    if(pTarget->isInFlight())
        return;

    // homebind location is loaded always
    pTarget->TeleportTo(pTarget->m_homebindMapId,pTarget->m_homebindX,pTarget->m_homebindY,pTarget->m_homebindZ,pTarget->GetOrientation());

    // Stuck spell trigger Hearthstone cooldown
    SpellEntry const *spellInfo = sSpellStore.LookupEntry(8690);
    if(!spellInfo)
        return;
    Spell spell(pTarget,spellInfo,true,0);
    spell.SendSpellCooldown();
}

void Spell::EffectSummonPlayer(uint32 /*i*/)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    // Evil Twin (ignore player summon, but hide this for summoner)
    if(unitTarget->GetDummyAura(23445))
        return;

    float x,y,z;
    m_caster->GetClosePoint(x,y,z);

    ((Player*)unitTarget)->SetSummonPoint(m_caster->GetMapId(),x,y,z);

    WorldPacket data(SMSG_SUMMON_REQUEST, 8+4+4);
    data << uint64(m_caster->GetGUID());                    // summoner guid
    data << uint32(m_caster->GetZoneId());                  // summoner zone
    data << uint32(MAX_PLAYER_SUMMON_DELAY*1000);           // auto decline after msecs
    ((Player*)unitTarget)->GetSession()->SendPacket(&data);
}

void Spell::EffectSummonTotem(uint32 i)
{
    uint8 slot = 0;
    switch(m_spellInfo->Effect[i])
    {
        case SPELL_EFFECT_SUMMON_TOTEM_SLOT1: slot = 0; break;
        case SPELL_EFFECT_SUMMON_TOTEM_SLOT2: slot = 1; break;
        case SPELL_EFFECT_SUMMON_TOTEM_SLOT3: slot = 2; break;
        case SPELL_EFFECT_SUMMON_TOTEM_SLOT4: slot = 3; break;
        // Battle standard case
        case SPELL_EFFECT_SUMMON_TOTEM:       slot = 254; break;
        // jewelery statue case, like totem without slot
        case SPELL_EFFECT_SUMMON_GUARDIAN:    slot = 255; break;
        default: return;
    }

    float angle = slot < 4 ? M_PI/4 - (slot*M_PI/2) : 0;

    float x,y,z;
    m_caster->GetClosePoint(x,y,z,2,angle);

    // totem must be at same Z in case swimming caster and etc.
    if( fabs( z - m_caster->GetPositionZ() ) > 5 )
        z = m_caster->GetPositionZ();

    if(slot <4 )
    {
        uint64 guid = m_caster->m_TotemSlot[slot];
        if(guid != 0)
        {
            Creature *OldTotem = ObjectAccessor::GetCreature(*m_caster, guid);
            if(OldTotem && OldTotem->isTotem())
                ((Totem*)OldTotem)->UnSummon();
        }
    }

    uint32 team = 0;
    if (m_caster->GetTypeId()==TYPEID_PLAYER)
        team = ((Player*)m_caster)->GetTeam();

    Totem* pTotem = new Totem(m_caster);

    if(!pTotem->Create(objmgr.GenerateLowGuid(HIGHGUID_UNIT),
        m_caster->GetMapId(), x, y, z, m_caster->GetOrientation(),
        m_spellInfo->EffectMiscValue[i], team ))
    {
        delete pTotem;
        return;
    }

    if(slot < 4)
        m_caster->m_TotemSlot[slot] = pTotem->GetGUID();

    pTotem->SetOwner(m_caster->GetGUID());
    pTotem->SetTypeBySummonSpell(m_spellInfo);              // must be after Create call where m_spells initilized

    int32 duration=GetSpellDuration(m_spellInfo);
    if(Player* modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id,SPELLMOD_DURATION, duration);
    pTotem->SetDuration(duration);

    pTotem->SetMaxHealth(damage);
    pTotem->SetHealth(damage);
    pTotem->SetUInt32Value(UNIT_CREATED_BY_SPELL,m_spellInfo->Id);

    pTotem->ApplySpellImmune(m_spellInfo->Id,IMMUNITY_STATE,SPELL_AURA_MOD_FEAR,true);
    pTotem->ApplySpellImmune(m_spellInfo->Id,IMMUNITY_STATE,SPELL_AURA_TRANSFORM,true);

    pTotem->Summon(m_caster);
}

void Spell::EffectEnchantHeldItem(uint32 i)
{
    // this is only item spell effect applied to main-hand weapon of target player (players in area)
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* item_owner = (Player*)unitTarget;
    Item* item = item_owner->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

    if(!item )
        return;

    // must be equipped
    if(!item ->IsEquipped())
        return;

    if (m_spellInfo->EffectMiscValue[i])
    {
        uint32 enchant_id = m_spellInfo->EffectMiscValue[i];
        int32 duration = GetSpellDuration(m_spellInfo);          //Try duration index first ..
        if(!duration)
            duration = m_currentBasePoints[i]+1;            //Base points after ..
        if(!duration)
            duration = 10;                                  //10 seconds for enchants which don't have listed duration

        SpellItemEnchantmentEntry const *pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if(!pEnchant)
            return;

        // Always go to temp enchantment slot
        EnchantmentSlot slot = TEMP_ENCHANTMENT_SLOT;

        // Enchantment will not be applied if a different one already exists
        if(item->GetEnchantmentId(slot) && item->GetEnchantmentId(slot) != enchant_id)
            return;

        // Apply the temporary enchantment
        item->SetEnchantment(slot, enchant_id, duration*1000, 0);
        item_owner->ApplyEnchantment(item,slot,true);
    }
}

void Spell::EffectDisEnchant(uint32 /*i*/)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;
    if(!itemTarget || !itemTarget->GetProto()->DisenchantID)
        return;

    p_caster->UpdateCraftSkill(m_spellInfo->Id);

    ((Player*)m_caster)->SendLoot(itemTarget->GetGUID(),LOOT_DISENCHANTING);

    // item will be removed at disenchanting end
}

void Spell::EffectInebriate(uint32 /*i*/)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *player = (Player*)unitTarget;
    uint16 currentDrunk = player->GetDrunkValue();
    uint16 drunkMod = damage * 0xFFFF / 100;
    if (currentDrunk + drunkMod > 0xFFFF)
        currentDrunk = 0xFFFF;
    else
        currentDrunk += drunkMod;
    player->SetDrunkValue(currentDrunk);
}

void Spell::EffectFeedPet(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)m_caster;

    if(!itemTarget)
        return;

    Pet *pet = _player->GetPet();
    if(!pet)
        return;

    if(!pet->isAlive())
        return;

    int32 benefit = pet->GetCurrentFoodBenefitLevel(itemTarget->GetProto()->ItemLevel);
    if(benefit <= 0)
        return;

    uint32 count = 1;
    _player->DestroyItemCount(itemTarget,count,true);
    // TODO: fix crash when a spell has two effects, both pointed at the same item target

    m_caster->CastCustomSpell(m_caster,m_spellInfo->EffectTriggerSpell[i],&benefit,NULL,NULL,true);
}

void Spell::EffectDismissPet(uint32 /*i*/)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Pet* pet = m_caster->GetPet();

    // not let dismiss dead pet
    if(!pet||!pet->isAlive())
        return;

    ((Player*)m_caster)->RemovePet(pet,PET_SAVE_NOT_IN_SLOT);
}

void Spell::EffectSummonObject(uint32 i)
{
    uint8 slot = 0;
    switch(m_spellInfo->Effect[i])
    {
        case SPELL_EFFECT_SUMMON_OBJECT_SLOT1: slot = 0; break;
        case SPELL_EFFECT_SUMMON_OBJECT_SLOT2: slot = 1; break;
        case SPELL_EFFECT_SUMMON_OBJECT_SLOT3: slot = 2; break;
        case SPELL_EFFECT_SUMMON_OBJECT_SLOT4: slot = 3; break;
        default: return;
    }

    uint64 guid = m_caster->m_ObjectSlot[slot];
    if(guid != 0)
    {
        GameObject* obj = NULL;
        if( m_caster )
            obj = ObjectAccessor::GetGameObject(*m_caster, guid);

        if(obj) obj->Delete();
        m_caster->m_ObjectSlot[slot] = 0;
    }

    GameObject* pGameObj = new GameObject(m_caster);
    uint32 display_id = m_spellInfo->EffectMiscValue[i];

    float rot2 = sin(m_caster->GetOrientation()/2);
    float rot3 = cos(m_caster->GetOrientation()/2);

    float x,y,z;
    m_caster->GetClosePoint(x,y,z);

    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), display_id,m_caster->GetMapId(), x, y, z, m_caster->GetOrientation(), 0, 0, rot2, rot3, 0, 1))
    {
        delete pGameObj;
        return;
    }

    pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL,m_caster->getLevel());
    int32 duration = GetSpellDuration(m_spellInfo);
    pGameObj->SetRespawnTime(duration > 0 ? duration/1000 : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);
    m_caster->AddGameObject(pGameObj);

    MapManager::Instance().GetMap(pGameObj->GetMapId(), pGameObj)->Add(pGameObj);
    WorldPacket data(SMSG_GAMEOBJECT_SPAWN_ANIM_OBSOLETE, 8);
    data << pGameObj->GetGUID();
    m_caster->SendMessageToSet(&data,true);

    m_caster->m_ObjectSlot[slot] = pGameObj->GetGUID();
}

void Spell::EffectResurrect(uint32 i)
{
    if(!unitTarget)
        return;
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if(unitTarget->isAlive())
        return;
    if(!unitTarget->IsInWorld())
        return;

    Player* pTarget = ((Player*)unitTarget);

    uint32 health = m_currentBasePoints[i]+1;
    uint32 mana = m_spellInfo->EffectMiscValue[i];

    ((Player*)unitTarget)->setResurrect(m_caster->GetGUID(), m_caster->GetMapId(), m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), health, mana);
    SendResurrectRequest(pTarget);
}

void Spell::EffectAddExtraAttacks(uint32 /*i*/)
{
    if(!unitTarget || !unitTarget->isAlive() || unitTarget->GetTypeId()!= TYPEID_PLAYER)
        return;

    if(((Player*)unitTarget)->m_extraAttacks )
        return;

    ((Player*)unitTarget)->m_extraAttacks = damage;
}

void Spell::EffectParry(uint32 /*i*/)
{
    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
    {
        ((Player*)unitTarget)->SetCanParry(true);
    }
}

void Spell::EffectMomentMove(uint32 i)
{
    if(unitTarget->isInFlight())
        return;

    if( m_spellInfo->rangeIndex== 1)                        //self range
    {
        uint32 mapid = m_caster->GetMapId();
        float dis = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

        // before caster
        float fx,fy,fz;
        unitTarget->GetClosePoint(fx,fy,fz,dis);
        float ox,oy,oz;
        unitTarget->GetPosition(ox,oy,oz);

        float fx2,fy2,fz2;                                  // getObjectHitPos overwrite last args in any result case
        if(VMAP::VMapFactory::createOrGetVMapManager()->getObjectHitPos(mapid, ox,oy,oz+0.5, fx,fy,oz+0.5,fx2,fy2,fz2, -0.5))
        {
            fx = fx2;
            fy = fy2;
            fz = fz2;
            unitTarget->UpdateGroundPositionZ(fx,fy,fz);
        }

        if(unitTarget->GetTypeId() == TYPEID_PLAYER)
            ((Player*)unitTarget)->TeleportTo(mapid, fx, fy, fz, unitTarget->GetOrientation(), false);
        else
            MapManager::Instance().GetMap(mapid, unitTarget)->CreatureRelocation((Creature*)unitTarget, fx, fy, fz, unitTarget->GetOrientation());
    }
}

void Spell::EffectReputation(uint32 i)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)unitTarget;

    int32  rep_change = m_currentBasePoints[i]+1;           // field store reputation change -1

    uint32 faction_id = m_spellInfo->EffectMiscValue[i];

    FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction_id);

    if(!factionEntry)
        return;

    _player->ModifyFactionReputation(factionEntry,rep_change);
}

void Spell::EffectQuestComplete(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)m_caster;

    uint32 quest_id = m_spellInfo->EffectMiscValue[i];

    _player->AreaExploredOrEventHappens(quest_id);
}

void Spell::EffectSelfResurrect(uint32 i)
{
    if(!unitTarget) return;
    if(unitTarget->GetTypeId() != TYPEID_PLAYER) return;
    if(unitTarget->isAlive()) return;
    if(!unitTarget->IsInWorld()) return;

    uint32 health = 0;
    uint32 mana = 0;

    // flat case
    if(damage < 0)
    {
        health = uint32(-damage);
        mana = m_spellInfo->EffectMiscValue[i];
    }
    // percent case
    else
    {
        health = uint32(damage/100.0f*unitTarget->GetMaxHealth());
        if(unitTarget->GetMaxPower(POWER_MANA) > 0)
            mana = uint32(damage/100.0f*unitTarget->GetMaxPower(POWER_MANA));
    }

    Player *plr = ((Player*)unitTarget);
    plr->ResurrectPlayer(0.0f);

    plr->SetHealth( health );
    plr->SetPower(POWER_MANA, mana );
    plr->SetPower(POWER_RAGE, 0 );
    plr->SetPower(POWER_ENERGY, plr->GetMaxPower(POWER_ENERGY) );

    plr->SpawnCorpseBones();

    plr->SaveToDB();
}

void Spell::EffectSkinning(uint32 /*i*/)
{
    if(unitTarget->GetTypeId() != TYPEID_UNIT )
        return;
    if(!m_caster || m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Creature* creature = (Creature*) unitTarget;
    int32 targetLevel = creature->getLevel();

    uint32 skill;
    if(creature->GetCreatureInfo()->flag1 & 256)
        skill = SKILL_HERBALISM;                            // special case
    else if(creature->GetCreatureInfo()->flag1 & 512)
        skill = SKILL_MINING;                               // special case
    else
        skill = SKILL_SKINNING;                             // normal case

    ((Player*)m_caster)->SendLoot(creature->GetGUID(),LOOT_SKINNING);
    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);

    int32 reqValue = targetLevel < 10 ? 0 : targetLevel < 20 ? (targetLevel-10)*10 : targetLevel*5;

    int32 skillValue = ((Player*)m_caster)->GetPureSkillValue(skill);

    // Double chances for elites
    ((Player*)m_caster)->UpdateGatherSkill(skill, skillValue, reqValue, creature->isElite() ? 2 : 1 );
}

void Spell::EffectCharge(uint32 /*i*/)
{
    if(!unitTarget || !m_caster)
        return;

    float x, y, z;
    unitTarget->GetContactPoint(m_caster, x, y, z);
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        ((Creature *)unitTarget)->StopMoving();

    // Only send MOVEMENTFLAG_WALK_MODE, client has strange issues with other move flags
    m_caster->SendMonsterMove(x, y, z, 0, MOVEMENTFLAG_WALK_MODE, 1);

    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        MapManager::Instance().GetMap(m_caster->GetMapId(), m_caster)->CreatureRelocation((Creature*)m_caster,x,y,z,m_caster->GetOrientation());
 
    // not all charge effects used in negative spells
    if ( !IsPositiveSpell(m_spellInfo->Id))
        m_caster->Attack(unitTarget,true);
}

void Spell::EffectSummonCritter(uint32 i)
{
    uint32 pet_entry = m_spellInfo->EffectMiscValue[i];
    if(!pet_entry)
        return;

    Unit* old_critter = NULL;

    {
        CellPair p(MaNGOS::ComputeCellPair(m_caster->GetPositionX(), m_caster->GetPositionY()));
        Cell cell(p);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        PetWithIdCheck u_check(m_caster, pet_entry);
        MaNGOS::UnitSearcher<PetWithIdCheck> checker(old_critter, u_check);
        TypeContainerVisitor<MaNGOS::UnitSearcher<PetWithIdCheck>, WorldTypeMapContainer > object_checker(checker);
        CellLock<GridReadGuard> cell_lock(cell, p);
        cell_lock->Visit(cell_lock, object_checker, *MapManager::Instance().GetMap(m_caster->GetMapId(), m_caster));
    }

    if (old_critter)                                        // find old critter, unsummon
    {
        // PetWithIdCheck return only Pets
        ((Pet*)old_critter)->Remove(PET_SAVE_AS_DELETED);
        return;
    }
    else                                                    // in another case summon new
    {
        Pet* critter = new Pet(m_caster, MINI_PET);

        // before caster
        float x,y,z;
        m_caster->GetClosePoint(x,y,z);

        uint32 pet_number = objmgr.GeneratePetNumber();
        if(!critter->Create(objmgr.GenerateLowGuid(HIGHGUID_PET),
            m_caster->GetMapId(),x,y,z,m_caster->GetOrientation(),m_spellInfo->EffectMiscValue[i], pet_number))
        {
            sLog.outError("no such creature entry %u",m_spellInfo->EffectMiscValue[i]);
            delete critter;
            return;
        }

        critter->SetUInt64Value(UNIT_FIELD_SUMMONEDBY,m_caster->GetGUID());
        critter->SetUInt64Value(UNIT_FIELD_CREATEDBY,m_caster->GetGUID());
        critter->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,m_caster->getFaction());
        critter->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);

        critter->AIM_Initialize();
        critter->InitPetCreateSpells();                     // e.g. disgusting oozeling has a create spell as critter...
        critter->SetMaxHealth(1);
        critter->SetHealth(1);
        critter->SetLevel(1);

        std::string name;
        if(m_caster->GetTypeId() == TYPEID_PLAYER)
            name = ((Player*)m_caster)->GetName();
        else
            name = ((Creature*)m_caster)->GetCreatureInfo()->Name;
        name.append(petTypeSuffix[critter->getPetType()]);
        critter->SetName( name );
        //m_caster->SetPet(critter);

        MapManager::Instance().GetMap(m_caster->GetMapId(), m_caster)->Add((Creature*)critter);
    }
}

void Spell::EffectKnockBack(uint32 i)
{
    if(!unitTarget || !m_caster)
        return;

    // Effect only works on players
    if(unitTarget->GetTypeId()!=TYPEID_PLAYER)
        return;

    float vsin = sin(m_caster->GetAngle(unitTarget));
    float vcos = cos(m_caster->GetAngle(unitTarget));

    WorldPacket data(SMSG_MOVE_KNOCK_BACK, (8+4+4+4+4+4));
    data.append(unitTarget->GetPackGUID());
    data << uint32(0);                                      // Sequence
    data << float(vcos);                                    // x direction
    data << float(vsin);                                    // y direction
    data << float(m_spellInfo->EffectMiscValue[i])/10;      // Horizontal speed
    data << float(damage/-10);                              // Z Movement speed (vertical)

    ((Player*)unitTarget)->GetSession()->SendPacket(&data);
}

void Spell::EffectPlayerPull(uint32 i)
{
    if(!unitTarget || !m_caster)
        return;

    // Effect only works on players
    if(unitTarget->GetTypeId()!=TYPEID_PLAYER)
        return;

    float vsin = sin(unitTarget->GetAngle(m_caster));
    float vcos = cos(unitTarget->GetAngle(m_caster));

    WorldPacket data(SMSG_MOVE_KNOCK_BACK, (8+4+4+4+4+4));
    data.append(unitTarget->GetPackGUID());
    data << uint32(0);                                      // Sequence
    data << float(vcos);                                    // x direction
    data << float(vsin);                                    // y direction
                                                            // Horizontal speed
    data << float(damage ? damage : unitTarget->GetDistance2d(m_caster));
    data << float(m_spellInfo->EffectMiscValue[i])/-10;     // Z Movement speed

    ((Player*)unitTarget)->GetSession()->SendPacket(&data);
}

void Spell::EffectDispelMechanic(uint32 i)
{
    if(!unitTarget) return;

    uint32 mechanic = m_spellInfo->EffectMiscValue[i];

    Unit::AuraMap& Auras = unitTarget->GetAuras();
    for(Unit::AuraMap::iterator iter = Auras.begin(), next; iter != Auras.end(); iter = next)
    {
        next = iter;
        ++next;
        SpellEntry const *spell = sSpellStore.LookupEntry(iter->second->GetSpellProto()->Id);
        if(spell->Mechanic == mechanic || spell->EffectMechanic[iter->second->GetEffIndex()] == mechanic)
        {
            unitTarget->RemoveAurasDueToSpell(spell->Id);
            if(Auras.empty())
                break;
            else
                next = Auras.begin();
        }
    }
    return;
}

void Spell::EffectSummonDeadPet(uint32 /*i*/)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    Player *_player = (Player*)m_caster;
    Pet *pet = _player->GetPet();
    if(!pet)
        return;
    if(pet->isAlive())
        return;
    if(damage < 0)
        return;
    pet->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
    pet->RemoveFlag (UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
    pet->setDeathState( ALIVE );
    pet->clearUnitState(UNIT_STAT_ALL_STATE);
    pet->SetHealth( uint32(pet->GetMaxHealth()*(float(damage)/100)));

    pet->AIM_Initialize();

    _player->PetSpellInitialize();
    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
}

void Spell::EffectDestroyAllTotems(uint32 /*i*/)
{
    float mana = 0;
    for(int slot = 0;  slot < 4; ++slot)
    {
        if(!m_caster->m_TotemSlot[slot])
            continue;

        Creature* totem = ObjectAccessor::GetCreature(*m_caster,m_caster->m_TotemSlot[slot]);
        if(totem && totem->isTotem())
        {
            uint32 spell_id = totem->GetUInt32Value(UNIT_CREATED_BY_SPELL);
            SpellEntry const* spellInfo = sSpellStore.LookupEntry(spell_id);
            if(spellInfo)
            {
                float percent = float(damage);
                mana += spellInfo->manaCost*percent/100;
            }
            ((Totem*)totem)->UnSummon();
        }
    }

    int32 gain = m_caster->ModifyPower(POWER_MANA,int32(mana));
    m_caster->SendEnergizeSpellLog(m_caster, m_spellInfo->Id, gain, POWER_MANA);
}

void Spell::EffectDurabilityDamage(uint32 i)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    int32 slot = m_spellInfo->EffectMiscValue[i];

    // FIXME: some spells effects have value -1/-2 (don't know what do for its :/ )
    if(slot < 0)
        return;

    // invalid slot value
    if(slot >= INVENTORY_SLOT_BAG_END)
        return;

    if(damage <= 0)
        return;

    ((Player*)unitTarget)->DurabilityPointsLoss(slot,damage);
}

void Spell::EffectDurabilityDamagePCT(uint32 i)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    int32 slot = m_spellInfo->EffectMiscValue[i];

    // FIXME: some spells effects have value -1/-2 (don't know what do for its :/ )
    if(slot < 0)
        return;

    // invalid slot value
    if(slot >= INVENTORY_SLOT_BAG_END)
        return;

    if(damage <= 0)
        return;

    ((Player*)unitTarget)->DurabilityLoss(slot,double(damage)/100);
}

void Spell::EffectReduceThreatPercent(uint32 /*i*/)
{
    if(!unitTarget)
        return;

    unitTarget->getHostilRefManager().addThreatPercent(-damage);
}

void Spell::EffectTransmitted(uint32 i)
{
    float fx,fy,fz;

    if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        fx = m_targets.m_destX;
        fy = m_targets.m_destY;
        fz = m_targets.m_destZ;
    }
    else
    {
        float min_dis = GetSpellMinRange(sSpellRangeStore.LookupEntry(m_spellInfo->rangeIndex));
        float max_dis = GetSpellMaxRange(sSpellRangeStore.LookupEntry(m_spellInfo->rangeIndex));
        float dis = rand_norm() * (max_dis - min_dis) + min_dis;

        m_caster->GetClosePoint(fx,fy,fz,dis);
    }

    uint32 cMap = m_caster->GetMapId();
    uint32 name_id = m_spellInfo->EffectMiscValue[i];
    GameObjectInfo const* goinfo = objmgr.GetGameObjectInfo(name_id);

    if (!goinfo)
    {
        sLog.outErrorDb("Gameobject (Entry: %u) not exist and not created at spell (ID: %u) cast",name_id, m_spellInfo->Id);
        return;
    }

    if(goinfo->type==GAMEOBJECT_TYPE_FISHINGNODE)
    {
        Map* map = MapManager::Instance().GetMap(cMap, m_caster);
        if ( !map->IsInWater(fx,fy,fz-0.5f)) // Hack to prevent fishing bobber from failing to land on fishing hole
        { // but this is not proper, we really need to ignore not materialized objects
            SendCastResult(SPELL_FAILED_NOT_HERE);
            SendChannelUpdate(0);
            return;
        }

        // replace by water level in this case
        fz = map->GetWaterLevel(fx,fy);
    }
    // if gameobject is summoning object, it should be spawned right on caster's position
    else if(goinfo->type==GAMEOBJECT_TYPE_SUMMONING_RITUAL)
    {
        m_caster->GetPosition(fx,fy,fz);
    }

    GameObject* pGameObj = new GameObject(m_caster);

    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), name_id,cMap,
        fx, fy, fz, m_caster->GetOrientation(), 0, 0, 0, 0, 100, 1))
    {
        delete pGameObj;
        return;
    }

    int32 duration = GetSpellDuration(m_spellInfo);

    switch(goinfo->type)
    {
        case GAMEOBJECT_TYPE_FISHINGNODE:
        {
            m_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,pGameObj->GetGUID());
                                                            // Orientation3
            pGameObj->SetFloatValue(GAMEOBJECT_ROTATION + 2, 0.88431775569915771 );
                                                            // Orientation4
            pGameObj->SetFloatValue(GAMEOBJECT_ROTATION + 3, -0.4668855369091033 );
            m_caster->AddGameObject(pGameObj);              // will removed at spell cancel

            // end time of range when possible catch fish (FISHING_BOBBER_READY_TIME..GetDuration(m_spellInfo))
            // start time == fish-FISHING_BOBBER_READY_TIME (0..GetDuration(m_spellInfo)-FISHING_BOBBER_READY_TIME)
            duration = urand(FISHING_BOBBER_READY_TIME*1000,duration);
            break;
        }
        case GAMEOBJECT_TYPE_SUMMONING_RITUAL:
        {
            if(m_caster->GetTypeId()==TYPEID_PLAYER)
                pGameObj->AddUniqueUse((Player*)m_caster);
            break;
        }
        case GAMEOBJECT_TYPE_FISHINGHOLE:
        case GAMEOBJECT_TYPE_CHEST:
        default:
        {
            break;
        }
    }

    pGameObj->SetRespawnTime(duration > 0 ? duration/1000 : 0);

    pGameObj->SetOwnerGUID(m_caster->GetGUID() );

    pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, m_caster->getLevel() );
    pGameObj->SetSpellId(m_spellInfo->Id);

    DEBUG_LOG("AddObject at SpellEfects.cpp EffectTransmitted\n");
    //m_caster->AddGameObject(pGameObj);
    //m_ObjToDel.push_back(pGameObj);

    MapManager::Instance().GetMap(cMap, pGameObj)->Add(pGameObj);

    WorldPacket data(SMSG_GAMEOBJECT_SPAWN_ANIM_OBSOLETE, 8);
    data << uint64(pGameObj->GetGUID());
    m_caster->SendMessageToSet(&data,true);

    if(uint32 linkedEntry = pGameObj->GetLinkedGameObjectEntry())
    {
        GameObject* linkedGO = new GameObject(m_caster);
        if(linkedGO->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), linkedEntry, cMap,
            fx, fy, fz, m_caster->GetOrientation(), 0, 0, 0, 0, 100, 1))
        {
            linkedGO->SetRespawnTime(duration > 0 ? duration/1000 : 0);
            linkedGO->SetUInt32Value(GAMEOBJECT_LEVEL, m_caster->getLevel() );
            linkedGO->SetSpellId(m_spellInfo->Id);
            linkedGO->SetOwnerGUID(m_caster->GetGUID() );

            MapManager::Instance().GetMap(linkedGO->GetMapId(), linkedGO)->Add(linkedGO);
        }
        else
        {
            delete linkedGO;
            linkedGO = NULL;
            return;
        }
    }
}

void Spell::EffectProspecting(uint32 /*i*/)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;
    if(!itemTarget || !(itemTarget->GetProto()->BagFamily & BAG_FAMILY_MASK_MINING_SUPP))
        return;

    if(itemTarget->GetCount() < 5)
        return;

    if( sWorld.getConfig(CONFIG_SKILL_PROSPECTING))
    {
        uint32 SkillValue = p_caster->GetPureSkillValue(SKILL_JEWELCRAFTING);
        uint32 reqSkillValue = itemTarget->GetProto()->RequiredSkillRank;
        p_caster->UpdateGatherSkill(SKILL_JEWELCRAFTING, SkillValue, reqSkillValue);
    }

    ((Player*)m_caster)->SendLoot(itemTarget->GetGUID(), LOOT_PROSPECTING);
}

void Spell::EffectSkill(uint32 /*i*/)
{
    sLog.outDebug("WORLD: SkillEFFECT");
}

void Spell::EffectApplyPetAura(uint32 i)
{
    // spell aura for both pet and owner, think the pet has the aura and applies it to owner
    EffectApplyAura(i);

    Unit* owner = unitTarget->GetCharmerOrOwner();
    if(owner)
    {
        unitTarget = owner;
        EffectApplyAura(i);
    }
}

void Spell::EffectSummonDemon(uint32 i)
{
    float px = m_targets.m_destX;
    float py = m_targets.m_destY;
    float pz = m_targets.m_destZ;

    if (px == 0 || py == 0 || pz == 0)
        m_caster->GetClosePoint(px, py, pz);

    Creature* Charmed = m_caster->SummonCreature(m_spellInfo->EffectMiscValue[i], px, py, pz, m_caster->GetOrientation(),TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,3600000);
    if (!Charmed)
        return;

    // might not always work correctly, maybe the creature that dies from CoD casts the effect on itself and is therefore the caster?
    Charmed->SetLevel(m_caster->getLevel());

    // TODO: Add damage/mana/hp according to level

    if (m_spellInfo->EffectMiscValue[i] == 89)              // Inferno summon
    {
        // Enslave demon effect, without mana cost and cooldown
        m_caster->CastSpell(Charmed, 20882, true);          // FIXME: enslave does not scale with level, level 62+ minions cannot be enslaved

        // Inferno effect
        Charmed->CastSpell(Charmed, 22703, true, 0);
    }
}

/* There is currently no need for this effect. We handle it in BattleGround.cpp
   If we would handle the resurrection here, the spiritguide would instantly disappear as the
   player revives, and so we wouldn't see the spirit heal visual effect on the npc.
   This is why we use a half sec delay between the visual effect and the resurrection itself */
void Spell::EffectSpiritHeal(uint32 /*i*/)
{
    /*
    if(!unitTarget) return;
    if(unitTarget->GetTypeId() != TYPEID_PLAYER) return;
    if(unitTarget->isAlive()) return;
    if(!unitTarget->IsInWorld()) return;

    //m_spellInfo->EffectBasePoints[i]; == 99 (percent?)
    //((Player*)unitTarget)->setResurrect(m_caster->GetGUID(), unitTarget->GetPositionX(), unitTarget->GetPositionY(), unitTarget->GetPositionZ(), unitTarget->GetMaxHealth(), unitTarget->GetMaxPower(POWER_MANA));
    ((Player*)unitTarget)->ResurrectPlayer(1.0f);
    ((Player*)unitTarget)->SpawnCorpseBones();
    */
}

// remove insignia spell effect
void Spell::EffectSkinPlayerCorpse(uint32 /*i*/)
{
    sLog.outDebug("Effect: SkinPlayerCorpse");
    if ( (m_caster->GetTypeId() != TYPEID_PLAYER) || (unitTarget->GetTypeId() != TYPEID_PLAYER) || (unitTarget->isAlive()) )
        return;

    ((Player*)unitTarget)->RemovedInsignia( (Player*)m_caster );
}

void Spell::EffectStealBeneficialBuff(uint32 /*i*/)
{
    sLog.outDebug("Effect: StealBeneficialBuff");

    // get the auras of the target
    Unit::AuraMap const& auras = unitTarget->GetAuras();

    // count possible for steal auras
    uint32 count = 0;
    for(Unit::AuraMap::const_iterator iter = auras.begin(); iter != auras.end(); ++iter)
    {
        if( iter->second->IsPositive() &&                   // only steel positive spell
            !iter->second->IsPassive() &&                   // don't steal passive abilities
            !iter->second->IsPersistent() &&                // don't steal persistent auras
            iter->second->GetSpellProto()->Dispel == m_spellInfo->Dispel )
                                                            // only steal magic effects
            ++count;
    }

    // not found any?
    if(!count)
        return;

    // random buff to steal
    uint32 remove_prev_positive = urand(0, count-1);

    // id of the spell we'll steal
    uint32 spellId = 0;

    // select aura by index
    count = 0;
    for(Unit::AuraMap::const_iterator iter = auras.begin(); iter != auras.end(); ++iter)
    {
        if( iter->second->IsPositive() &&                   // only steel positive spell
            !iter->second->IsPassive() &&                   // don't steal passive abilities
            !iter->second->IsPersistent() &&                // don't steal persistent auras
            iter->second->GetSpellProto()->Dispel == m_spellInfo->Dispel )
                                                            // only steal magic effects
        {
            if(count==remove_prev_positive)
            {
                spellId = iter->first.first;                // store the id of the last stealable spell
                break;
            }

            ++count;
        }
    }

    // we found a stealable buff
    if(spellId)
    {
        // max duration 2 minutes (in msecs)
        const int32 max_dur = 2*MINUTE*1000;

        // go through all the effects of the spell
        for(int i=0; i<3; ++i)
        {
            // get the corresponding aura from the victim
            Aura * aur = unitTarget->GetAura(spellId, i);

            if(!aur)
                continue;

            // we have to check against the (remaining) duration on the victim
            int32 dur        = aur->GetAuraDuration();
            int32 basePoints = aur->GetBasePoints();

            // construct the new aura for the attacker
            Aura * new_aur = CreateAura(aur->GetSpellProto(), i, &basePoints, m_caster);

            if(!new_aur)
                continue;

            // set its duration and maximum duration
            new_aur->SetAuraMaxDuration( max_dur > dur ? dur : max_dur );
            new_aur->SetAuraDuration( max_dur > dur ? dur : max_dur );

            // add the new aura
            m_caster->AddAura(new_aur);
        }

        // remove the auras caused by the stolen spell from the victim
        unitTarget->RemoveAurasDueToSpell(spellId);
    }
}
