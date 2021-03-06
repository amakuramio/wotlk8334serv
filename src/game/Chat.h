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

#ifndef MANGOSSERVER_CHAT_H
#define MANGOSSERVER_CHAT_H

#include "SharedDefines.h"

class ChatHandler;
class WorldSession;
class Creature;
class Player;
class Unit;

struct LanguageDesc
{
    Language lang_id;
    uint32   spell_id;
    uint32   skill_id;
};

extern LanguageDesc lang_description[LANGUAGES_COUNT];

LanguageDesc const* GetLanguageDescByID(uint32 lang);
LanguageDesc const* GetLanguageDescBySpell(uint32 spell_id);
LanguageDesc const* GetLanguageDescBySkill(uint32 skill_id);

class ChatCommand
{
    public:
        const char *       Name;
        uint32             SecurityLevel;                   // function pointer required correct align (use uint32)
        bool (ChatHandler::*Handler)(const char* args);
        std::string        Help;
        ChatCommand *      ChildCommands;
};

class ChatHandler
{
    public:
        explicit ChatHandler(WorldSession* session) : m_session(session) {}
        explicit ChatHandler(Player* player) : m_session(player->GetSession()) {}
        ~ChatHandler() {}

        static void FillMessageData( WorldPacket *data, WorldSession* session, uint8 type, uint32 language, const char *channelName, uint64 target_guid, const char *message, Unit *speaker);

        void FillMessageData( WorldPacket *data, uint8 type, uint32 language, uint64 target_guid, const char* message)
        {
            FillMessageData( data, m_session, type, language, NULL, target_guid, message, NULL);
        }

        void FillSystemMessageData( WorldPacket *data, const char* message )
        {
            FillMessageData( data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, 0, message );
        }

        static char* LineFromMessage(char*& pos) { char* start = strtok(pos,"\n"); pos = NULL; return start; }

        const char *GetMangosString(uint32 entry);

        void SendSysMessage(          const char *str);
        void SendSysMessage(          uint32     entry);
        void PSendSysMessage(         const char *format, ...) ATTR_PRINTF(2,3);
        void PSendSysMessage(         uint32     entry, ...  );

        int ParseCommands(const char* text);

    protected:
        bool hasStringAbbr(const char* s1, const char* s2);
        void SendGlobalSysMessage(const char *str);

        bool ExecuteCommandInTable(ChatCommand *table, const char* text, std::string fullcommand);
        bool ShowHelpForCommand(ChatCommand *table, const char* cmd);
        bool ShowHelpForSubCommands(ChatCommand *table, char const* cmd, char const* subcmd);

        ChatCommand* getCommandTable();

        bool HandleHelpCommand(const char* args);
        bool HandleCommandsCommand(const char* args);
        bool HandleAcctCommand(const char* args);
        bool HandleStartCommand(const char* args);
        bool HandleInfoCommand(const char* args);
        bool HandleDismountCommand(const char* args);
        bool HandleSaveCommand(const char* args);
        bool HandleGMListCommand(const char* args);

        bool HandleNamegoCommand(const char* args);
        bool HandleGonameCommand(const char* args);
        bool HandleGroupgoCommand(const char* args);
        bool HandleRecallCommand(const char* args);
        bool HandleAnnounceCommand(const char* args);
        bool HandleNotifyCommand(const char* args);
        bool HandleGMmodeCommand(const char* args);
        bool HandleVisibleCommand(const char* args);
        bool HandleGPSCommand(const char* args);
        bool HandleTaxiCheatCommand(const char* args);
        bool HandleWhispersCommand(const char* args);
        bool HandleSayCommand(const char* args);
        bool HandleNpcWhisperCommand(const char* args);
        bool HandleYellCommand(const char* args);
        bool HandlePlayEmoteCommand(const char* args);
        bool HandleSendMailCommand(const char* args);
        bool HandleNameTeleCommand(const char* args);
        bool HandleGroupTeleCommand(const char* args);
        bool HandleDrunkCommand(const char* args);

        bool HandleModifyKnownTitlesCommand(const char* args);
        bool HandleModifyHPCommand(const char* args);
        bool HandleModifyManaCommand(const char* args);
        bool HandleModifyRageCommand(const char* args);
        bool HandleModifyEnergyCommand(const char* args);
        bool HandleModifyMoneyCommand(const char* args);
        bool HandleModifyASpeedCommand(const char* args);
        bool HandleModifySpeedCommand(const char* args);
        bool HandleModifyBWalkCommand(const char* args);
        bool HandleModifyFlyCommand(const char* args);
        bool HandleModifySwimCommand(const char* args);
        bool HandleModifyScaleCommand(const char* args);
        bool HandleModifyMountCommand(const char* args);
        bool HandleModifyBitCommand(const char* args);
        bool HandleModifyFactionCommand(const char* args);
        bool HandleModifySpellCommand(const char* args);
        bool HandleModifyTalentCommand (const char* args);
        bool HandleModifyHonorCommand (const char* args);
        bool HandleModifyRepCommand(const char* args);
        bool HandleModifyArenaCommand(const char* args);

        bool HandleReloadCommand(const char* args);
        bool HandleReloadAllCommand(const char* args);
        bool HandleReloadAllAreaCommand(const char* args);
        bool HandleReloadAllQuestCommand(const char* args);
        bool HandleReloadAllLootCommand(const char* args);
        bool HandleReloadAllScriptsCommand(const char* args);
        bool HandleReloadAllSpellCommand(const char* args);

        bool HandleReloadAreaTriggerTavernCommand(const char* args);
        bool HandleReloadAreaTriggerTeleportCommand(const char* args);
        bool HandleReloadEventScriptsCommand(const char* args);
        bool HandleReloadCommandCommand(const char* args);
        bool HandleReloadCreatureQuestRelationsCommand(const char* args);
        bool HandleReloadCreatureQuestInvRelationsCommand(const char* args);
        bool HandleReloadGameGraveyardZoneCommand(const char* args);
        bool HandleReloadGameObjectScriptsCommand(const char* args);
        bool HandleReloadGOQuestRelationsCommand(const char* args);
        bool HandleReloadGOQuestInvRelationsCommand(const char* args);
        bool HandleReloadLootTemplatesCreatureCommand(const char* args);
        bool HandleReloadLootTemplatesDisenchantCommand(const char* args);
        bool HandleReloadLootTemplatesFishingCommand(const char* args);
        bool HandleReloadLootTemplatesGameobjectCommand(const char* args);
        bool HandleReloadLootTemplatesItemCommand(const char* args);
        bool HandleReloadLootTemplatesPickpocketingCommand(const char* args);
        bool HandleReloadLootTemplatesSkinningCommand(const char* args);
        bool HandleReloadLootTemplatesProspectingCommand(const char* args);
        bool HandleReloadMangosStringCommand(const char* args);
        bool HandleReloadQuestAreaTriggersCommand(const char* args);
        bool HandleReloadQuestEndScriptsCommand(const char* args);
        bool HandleReloadQuestStartScriptsCommand(const char* args);
        bool HandleReloadQuestTemplateCommand(const char* args);
        bool HandleReloadReservedNameCommand(const char*);
        bool HandleReloadSkillDiscoveryTemplateCommand(const char* args);
        bool HandleReloadSkillExtraItemTemplateCommand(const char* args);
        bool HandleReloadSkillFishingBaseLevelCommand(const char* args);
        bool HandleReloadSpellAffectCommand(const char* args);
        bool HandleReloadSpellChainCommand(const char* args);
        bool HandleReloadSpellElixirCommand(const char* args);
        bool HandleReloadSpellLearnSpellCommand(const char* args);
        bool HandleReloadSpellProcEventCommand(const char* args);
        bool HandleReloadSpellScriptTargetCommand(const char* args);
        bool HandleReloadSpellScriptsCommand(const char* args);
        bool HandleReloadSpellTeleportCommand(const char* args);
        bool HandleReloadSpellThreatsCommand(const char* args);

        bool HandleAddHonorCommand(const char* args);
        bool HandleHonorAddKillCommand(const char* args);
        bool HandleUpdateHonorFieldsCommand(const char* args);

        bool HandleLoadScriptsCommand(const char* args);
        bool HandleSendQuestPartyMsgCommand(const char* args);
        bool HandleSendQuestInvalidMsgCommand(const char* args);

        bool HandleDebugInArcCommand(const char* args);
        bool HandleDebugSpellFailCommand(const char* args);

        bool HandleGUIDCommand(const char* args);
        bool HandleNameCommand(const char* args);
        bool HandleSubNameCommand(const char* args);
        bool HandleItemMoveCommand(const char* args);
        bool HandleDeleteCommand(const char* args);
        bool HandleDeMorphCommand(const char* args);
        bool HandleAddVendorItemCommand(const char* args);
        bool HandleDelVendorItemCommand(const char* args);
        bool HandleAddMoveCommand(const char* args);
        bool HandleSetMoveTypeCommand(const char* args);
        bool HandleChangeLevelCommand(const char* args);
        bool HandleSetPoiCommand(const char* args);
        bool HandleEquipErrorCommand(const char* args);
        bool HandleNPCFlagCommand(const char* args);
        bool HandleSetModelCommand(const char* args);
        bool HandleFactionIdCommand(const char* args);
        bool HandleAddSpwCommand(const char* args);
        bool HandleSpawnDistCommand(const char* args);
        bool HandleSpawnTimeCommand(const char* args);
        bool HandleGoCreatureCommand(const char* args);
        bool HandleGoObjectCommand(const char* args);
        bool HandleGoTriggerCommand(const char* args);
        bool HandleGoGraveyardCommand(const char* args);
        bool HandleTargetObjectCommand(const char* args);
        bool HandleDelObjectCommand(const char* args);
        bool HandleMoveCreatureCommand(const char* args);
        bool HandleMoveObjectCommand(const char* args);
        bool HandleTurnObjectCommand(const char* args);
        bool HandlePInfoCommand(const char* args);
        bool HandlePLimitCommand(const char* args);
        bool HandleMuteCommand(const char* args);
        bool HandleUnmuteCommand(const char* args);
        bool HandleMovegensCommand(const char* args);

        bool HandleBanCommand(const char* args);
        bool HandleUnBanCommand(const char* args);
        bool HandleBanInfoCommand(const char* args);
        bool HandleBanListCommand(const char* args);
        bool HandleIdleShutDownCommand(const char* args);
        bool HandleShutDownCommand(const char* args);
        bool HandleSecurityCommand(const char* args);
        bool HandleGoXYCommand(const char* args);
        bool HandleGoXYZCommand(const char* args);
        bool HandleGoZoneXYCommand(const char* args);
        bool HandleGoGridCommand(const char* args);
        bool HandleAddWeaponCommand(const char* args);
        bool HandleAllowMovementCommand(const char* args);
        bool HandleGoCommand(const char* args);
        bool HandleLearnCommand(const char* args);
        bool HandleLearnAllCommand(const char* args);
        bool HandleLearnAllGMCommand(const char* args);
        bool HandleLearnAllCraftsCommand(const char* args);
        bool HandleLearnAllDefaultCommand(const char* args);
        bool HandleLearnAllLangCommand(const char* args);
        bool HandleLearnAllMyClassCommand(const char* args);
        bool HandleLearnAllMySpellsCommand(const char* args);
        bool HandleLearnAllMyTalentsCommand(const char* args);
        bool HandleCooldownCommand(const char* args);
        bool HandleUnLearnCommand(const char* args);
        bool HandleGetDistanceCommand(const char* args);
        bool HandleGameObjectCommand(const char* args);
        bool HandleAnimCommand(const char* args);
        bool HandlePlaySoundCommand(const char* args);
        bool HandleStandStateCommand(const char* args);
        bool HandleDieCommand(const char* args);
        bool HandleDamageCommand(const char *args);
        bool HandleReviveCommand(const char* args);
        bool HandleMorphCommand(const char* args);
        bool HandleAuraCommand(const char* args);
        bool HandleUnAuraCommand(const char* args);
        bool HandleLinkGraveCommand(const char* args);
        bool HandleNearGraveCommand(const char* args);
        bool HandleSpawnTransportCommand(const char* args);
        bool HandleExploreCheatCommand(const char* args);
        bool HandleTextEmoteCommand(const char* args);
        bool HandleNpcInfoCommand(const char* args);
        bool HandleHoverCommand(const char* args);
        bool HandleLevelUpCommand(const char* args);
        bool HandleShowAreaCommand(const char* args);
        bool HandleHideAreaCommand(const char* args);
        bool HandleAddItemCommand(const char* args);
        bool HandleAddItemSetCommand(const char* args);
        bool HandleLookupItemCommand(const char * args);
        bool HandleLookupItemSetCommand(const char * args);
        bool HandleGuildCreateCommand(const char* args);
        bool HandleGuildInviteCommand(const char* args);
        bool HandleGuildUninviteCommand(const char* args);
        bool HandleGuildRankCommand(const char* args);
        bool HandleGuildDeleteCommand(const char* args);
        bool HandleUpdate(const char* args);
        bool HandleBankCommand(const char* args);
        bool HandleChangeWeather(const char* args);
        bool HandleKickPlayerCommand(const char * args);
        bool HandleTeleCommand(const char * args);
        bool HandleAddTeleCommand(const char * args);
        bool HandleDelTeleCommand(const char * args);
        bool HandleListAurasCommand(const char * args);
        bool HandleLookupTeleCommand(const char * args);

        bool HandleResetHonorCommand(const char * args);
        bool HandleResetLevelCommand(const char * args);
        bool HandleResetSpellsCommand(const char * args);

        bool HandleResetStatsCommand(const char * args);
        bool HandleResetTalentsCommand(const char * args);

        bool HandleResetAllCommand(const char * args);
        bool HandleTicketCommand(const char* args);
        bool HandleDelTicketCommand(const char* args);
        bool HandleMaxSkillCommand(const char* args);
        bool HandleSetSkillCommand(const char* args);
        bool HandleListCreatureCommand(const char* args);
        bool HandleListItemCommand(const char* args);
        bool HandleListObjectCommand(const char* args);
        bool HandleNearObjectCommand(const char* args);
        bool HandleLookupAreaCommand(const char* args);
        bool HandleLookupFactionCommand(const char * args);
        bool HandleLookupSkillCommand(const char* args);
        bool HandleLookupSpellCommand(const char* args);
        bool HandleLookupQuestCommand(const char* args);
        bool HandleLookupCreatureCommand(const char* args);
        bool HandleLookupObjectCommand(const char* args);
        bool HandlePasswordCommand(const char* args);
        bool HandleLockAccountCommand(const char* args);
        bool HandleRespawnCommand(const char* args);
        bool HandleWpAddCommand(const char* args);
        bool HandleWpModifyCommand(const char* args);
        bool HandleWpShowCommand(const char* args);
        bool HandleWpExportCommand(const char* args);
        bool HandleWpImportCommand(const char* args);
        bool HandleFlyModeCommand(const char* args);
        bool HandleSendOpcodeCommand(const char* args);
        bool HandleSellErrorCommand(const char* args);
        bool HandleBuyErrorCommand(const char* args);
        bool HandleUpdateWorldStateCommand(const char* args);
        bool HandlePlaySound2Command(const char* args);
        bool HandleSendChannelNotifyCommand(const char* args);
        bool HandleSendChatMsgCommand(const char* args);
        bool HandleRenameCommand(const char * args);
        bool HandleLoadPDumpCommand(const char *args);
        bool HandleWritePDumpCommand(const char *args);
        bool HandleChangeEntryCommand(const char *args);
        bool HandleCastCommand(const char *args);
        bool HandleCastBackCommand(const char *args);
        bool HandleCastSelfCommand(const char *args);
        bool HandleCastTargetCommand(const char *args);
        bool HandleComeToMeCommand(const char *args);

        //! Development Commands
        bool HandleSetValue(const char* args);
        bool HandleGetValue(const char* args);
        bool HandleSet32Bit(const char* args);
        bool HandleMod32Value(const char* args);
        bool HandleAddQuest(const char * args);
        bool HandleRemoveQuest(const char * args);
        bool HandleSaveAllCommand(const char* args);
        bool HandleGetItemState(const char * args);

        Player*   getSelectedPlayer();
        Creature* getSelectedCreature();
        Unit*     getSelectedUnit();
        char*     extractKeyFromLink(char* text, char const* linkType);

        GameObject* GetObjectGlobalyWithGuidOrNearWithDbGuid(uint32 lowguid,uint32 entry);

        WorldSession * m_session;

        // Utility methods for commands
        void ShowTicket(uint64 guid, uint32 category, char const* text, char const* time);
        uint32 GetTicketIDByNum(uint32 num);

        // common global flag
        static bool load_command_table;
};
#endif

char const *fmtstring( char const *format, ... );
