/*
===========================================================================
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

#include "g_local.h"
#include "b_local.h"
#include "g_functions.h"
#include "wp_saber.h"
#include "w_local.h"

//-------------------
//	Heavy Repeater
//-------------------

//---------------------------------------------------------
static void WP_RepeaterMainFire( gentity_t *ent, vec3_t dir )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage	= weaponData[WP_REPEATER].damage;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	WP_MissileTargetHint(ent, start, dir);

	gentity_t *missile = CreateMissile( start, dir, REPEATER_VELOCITY, 10000, ent );

	missile->classname = "repeater_proj";
	missile->s.weapon = WP_REPEATER;

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = REPEATER_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = REPEATER_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = REPEATER_NPC_DAMAGE_HARD;
		}
	}

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_REPEATER;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

static void WP_CloneRifleMainFire(gentity_t* ent, vec3_t dir)
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage = weaponData[WP_CLONERIFLE].damage;

	VectorCopy(muzzle, start);
	WP_TraceSetStart(ent, start, vec3_origin, vec3_origin);//make sure our start point isn't on the other side of a wall

	WP_MissileTargetHint(ent, start, dir);

	gentity_t* missile = CreateMissile(start, dir, CLONERIFLE_VELOCITY, 10300, ent);

	missile->classname = "clonerifle_proj";
	missile->s.weapon = WP_CLONERIFLE;

	// Do the damages
	if (ent->s.number != 0)
	{
		if (g_spskill->integer == 0)
		{
			damage = CLONERIFLE_NPC_DAMAGE_EASY;
		}
		else if (g_spskill->integer == 1)
		{
			damage = CLONERIFLE_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = CLONERIFLE_NPC_DAMAGE_HARD;
		}
	}

	//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
	//	{
	//		// in overcharge mode, so doing double damage
	//		missile->flags |= FL_OVERCHARGED;
	//		damage *= 2;
	//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_CLONERIFLE;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 10;
}

static void WP_RebelRifleMainFire(gentity_t* ent, vec3_t dir)
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage = weaponData[WP_REBELRIFLE].damage;

	VectorCopy(muzzle, start);
	WP_TraceSetStart(ent, start, vec3_origin, vec3_origin);//make sure our start point isn't on the other side of a wall

	WP_MissileTargetHint(ent, start, dir);

	gentity_t* missile = CreateMissile(start, dir, REBELRIFLE_VELOCITY, 10000, ent);

	missile->classname = "rebelrifle_proj";
	missile->s.weapon = WP_REBELRIFLE;

	// Do the damages
	if (ent->s.number != 0)
	{
		if (g_spskill->integer == 0)
		{
			damage = REBELRIFLE_NPC_DAMAGE_EASY;
		}
		else if (g_spskill->integer == 1)
		{
			damage = REBELRIFLE_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = REBELRIFLE_NPC_DAMAGE_HARD;
		}
	}

	//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
	//	{
	//		// in overcharge mode, so doing double damage
	//		missile->flags |= FL_OVERCHARGED;
	//		damage *= 2;
	//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_REBELRIFLE;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 6;
}

//---------------------------------------------------------
static void WP_RepeaterAltFire( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage	= weaponData[WP_REPEATER].altDamage;
	gentity_t *missile = NULL;

	VectorCopy( muzzle, start );
	WP_TraceSetStart( ent, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	if ( ent->client && ent->client->NPC_class == CLASS_GALAKMECH )
	{
		missile = CreateMissile( start, ent->client->hiddenDir, ent->client->hiddenDist, 10000, ent, qtrue );
	}
	else
	{
		WP_MissileTargetHint(ent, start, forwardVec);
		missile = CreateMissile( start, forwardVec, REPEATER_ALT_VELOCITY, 10000, ent, qtrue );
	}

	missile->classname = "repeater_alt_proj";
	missile->s.weapon = WP_REPEATER;
	missile->mass = 10;

	// Do the damages
	if ( ent->s.number != 0 )
	{
		if ( g_spskill->integer == 0 )
		{
			damage = REPEATER_ALT_NPC_DAMAGE_EASY;
		}
		else if ( g_spskill->integer == 1 )
		{
			damage = REPEATER_ALT_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = REPEATER_ALT_NPC_DAMAGE_HARD;
		}
	}

	VectorSet( missile->maxs, REPEATER_ALT_SIZE, REPEATER_ALT_SIZE, REPEATER_ALT_SIZE );
	VectorScale( missile->maxs, -1, missile->mins );
	missile->s.pos.trType = TR_GRAVITY;
	missile->s.pos.trDelta[2] += 40.0f; //give a slight boost in the upward direction

//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
//	{
//		// in overcharge mode, so doing double damage
//		missile->flags |= FL_OVERCHARGED;
//		damage *= 2;
//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_REPEATER_ALT;
	missile->splashMethodOfDeath = MOD_REPEATER_ALT;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
	missile->splashDamage = weaponData[WP_REPEATER].altSplashDamage;
	missile->splashRadius = weaponData[WP_REPEATER].altSplashRadius;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

static void WP_CloneRifleAltFire(gentity_t* ent)
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage = weaponData[WP_CLONERIFLE].altDamage;
	gentity_t* missile = NULL;

	VectorCopy(muzzle, start);
	WP_TraceSetStart(ent, start, vec3_origin, vec3_origin);//make sure our start point isn't on the other side of a wall

	if (ent->client && ent->client->NPC_class == CLASS_GALAKMECH)
	{
		missile = CreateMissile(start, ent->client->hiddenDir, ent->client->hiddenDist, 10000, ent, qtrue);
	}
	else
	{
		WP_MissileTargetHint(ent, start, forwardVec);
		missile = CreateMissile(start, forwardVec, CLONERIFLE_ALT_VELOCITY, 10000, ent, qtrue);
	}

	missile->classname = "clonerifle_alt_proj";
	missile->s.weapon = WP_CLONERIFLE;
	missile->mass = 10;

	// Do the damages
	if (ent->s.number != 0)
	{
		if (g_spskill->integer == 0)
		{
			damage = CLONERIFLE_ALT_NPC_DAMAGE_EASY;
		}
		else if (g_spskill->integer == 1)
		{
			damage = CLONERIFLE_ALT_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = CLONERIFLE_ALT_NPC_DAMAGE_HARD;
		}
	}

	VectorSet(missile->maxs, CLONERIFLE_ALT_SIZE, CLONERIFLE_ALT_SIZE, CLONERIFLE_ALT_SIZE);
	VectorScale(missile->maxs, -1, missile->mins);
	missile->s.pos.trType = TR_GRAVITY;
	missile->s.pos.trDelta[2] += 40.0f; //give a slight boost in the upward direction

	//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
	//	{
	//		// in overcharge mode, so doing double damage
	//		missile->flags |= FL_OVERCHARGED;
	//		damage *= 2;
	//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_CLONERIFLE_ALT;
	missile->splashMethodOfDeath = MOD_CLONERIFLE_ALT;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
	missile->splashDamage = weaponData[WP_CLONERIFLE].altSplashDamage;
	missile->splashRadius = weaponData[WP_CLONERIFLE].altSplashRadius;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

static void WP_RebelRifleAltFire(gentity_t* ent)
//---------------------------------------------------------
{
	vec3_t	start;
	int		damage = weaponData[WP_REBELRIFLE].altDamage;
	gentity_t* missile = NULL;

	VectorCopy(muzzle, start);
	WP_TraceSetStart(ent, start, vec3_origin, vec3_origin);//make sure our start point isn't on the other side of a wall

	if (ent->client && ent->client->NPC_class == CLASS_GALAKMECH)
	{
		missile = CreateMissile(start, ent->client->hiddenDir, ent->client->hiddenDist, 10000, ent, qtrue);
	}
	else
	{
		WP_MissileTargetHint(ent, start, forwardVec);
		missile = CreateMissile(start, forwardVec, REBELRIFLE_ALT_VELOCITY, 10000, ent, qtrue);
	}

	missile->classname = "rebelrifle_alt_proj";
	missile->s.weapon = WP_REBELRIFLE;
	missile->mass = 10;

	// Do the damages
	if (ent->s.number != 0)
	{
		if (g_spskill->integer == 0)
		{
			damage = REBELRIFLE_ALT_NPC_DAMAGE_EASY;
		}
		else if (g_spskill->integer == 1)
		{
			damage = REBELRIFLE_ALT_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = REBELRIFLE_ALT_NPC_DAMAGE_HARD;
		}
	}

	VectorSet(missile->maxs, REBELRIFLE_ALT_SIZE, REBELRIFLE_ALT_SIZE, REBELRIFLE_ALT_SIZE);
	VectorScale(missile->maxs, -1, missile->mins);
	missile->s.pos.trType = TR_GRAVITY;
	missile->s.pos.trDelta[2] += 40.0f; //give a slight boost in the upward direction

	//	if ( ent->client && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > 0 && ent->client->ps.powerups[PW_WEAPON_OVERCHARGE] > cg.time )
	//	{
	//		// in overcharge mode, so doing double damage
	//		missile->flags |= FL_OVERCHARGED;
	//		damage *= 2;
	//	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_REBELRIFLE_ALT;
	missile->splashMethodOfDeath = MOD_REBELRIFLE_ALT;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
	missile->splashDamage = weaponData[WP_REBELRIFLE].altSplashDamage;
	missile->splashRadius = weaponData[WP_REBELRIFLE].altSplashRadius;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

//---------------------------------------------------------
void WP_FireRepeater( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	vec3_t	dir, angs;

	vectoangles( forwardVec, angs );

	if ( alt_fire )
	{
		WP_RepeaterAltFire( ent );
	}
	else
	{
		if ( !(ent->client->ps.forcePowersActive&(1<<FP_SEE))
			|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2 )
		{//force sight 2+ gives perfect aim
			//FIXME: maybe force sight level 3 autoaims some?
			// Troopers use their aim values as well as the gun's inherent inaccuracy
			// so check for all classes of stormtroopers and anyone else that has aim error
			if ( ent->client && ent->NPC &&
				( ent->client->NPC_class == CLASS_STORMTROOPER ||
				  ent->client->NPC_class == CLASS_SWAMPTROOPER ||
				  ent->client->NPC_class == CLASS_SHADOWTROOPER ) )
			{
				angs[PITCH] += ( Q_flrand(-1.0f, 1.0f) * (REPEATER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f) );
				angs[YAW]	+= ( Q_flrand(-1.0f, 1.0f) * (REPEATER_NPC_SPREAD+(6-ent->NPC->currentAim)*0.25f) );
			}
			else
			{
				// add some slop to the alt-fire direction
				angs[PITCH] += Q_flrand(-1.0f, 1.0f) * REPEATER_SPREAD;
				angs[YAW]	+= Q_flrand(-1.0f, 1.0f) * REPEATER_SPREAD;
			}
		}

		AngleVectors( angs, dir, NULL, NULL );

		// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
		WP_RepeaterMainFire( ent, dir );
	}
}

void WP_FireCloneRifle(gentity_t* ent, qboolean alt_fire)
//---------------------------------------------------------
{
	vec3_t	dir, angs;

	vectoangles(forwardVec, angs);

	if (alt_fire)
	{
		WP_CloneRifleAltFire(ent);
	}
	else
	{
		if (!(ent->client->ps.forcePowersActive & (1 << FP_SEE))
			|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2)
		{//force sight 2+ gives perfect aim
			//FIXME: maybe force sight level 3 autoaims some?
			// Troopers use their aim values as well as the gun's inherent inaccuracy
			// so check for all classes of stormtroopers and anyone else that has aim error
			if (ent->client && ent->NPC &&
				(ent->client->NPC_class == CLASS_STORMTROOPER ||
					ent->client->NPC_class == CLASS_SWAMPTROOPER ||
					ent->client->NPC_class == CLASS_CLONE ||
					ent->client->NPC_class == CLASS_SHADOWTROOPER))
			{
				angs[PITCH] += (Q_flrand(-1.0f, 1.0f) * (CLONERIFLE_NPC_SPREAD + (6 - ent->NPC->currentAim) * 0.25f));
				angs[YAW] += (Q_flrand(-1.0f, 1.0f) * (CLONERIFLE_NPC_SPREAD + (6 - ent->NPC->currentAim) * 0.25f));
			}
			else
			{
				// add some slop to the alt-fire direction
				angs[PITCH] += Q_flrand(-1.0f, 1.0f) * CLONERIFLE_SPREAD;
				angs[YAW] += Q_flrand(-1.0f, 1.0f) * CLONERIFLE_SPREAD;
			}
		}

		AngleVectors(angs, dir, NULL, NULL);

		// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
		WP_CloneRifleMainFire(ent, dir);
	}
}

void WP_FireRebelRifle(gentity_t* ent, qboolean alt_fire)
//---------------------------------------------------------
{
	vec3_t	dir, angs;

	vectoangles(forwardVec, angs);

	if (alt_fire)
	{
		WP_RebelRifleAltFire(ent);
	}
	else
	{
		if (!(ent->client->ps.forcePowersActive & (1 << FP_SEE))
			|| ent->client->ps.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2)
		{//force sight 2+ gives perfect aim
			//FIXME: maybe force sight level 3 autoaims some?
			// Troopers use their aim values as well as the gun's inherent inaccuracy
			// so check for all classes of stormtroopers and anyone else that has aim error
			if (ent->client && ent->NPC &&
				(ent->client->NPC_class == CLASS_STORMTROOPER ||
					ent->client->NPC_class == CLASS_SWAMPTROOPER ||
					ent->client->NPC_class == CLASS_SHADOWTROOPER))
			{
				angs[PITCH] += (Q_flrand(-1.0f, 1.0f) * (REBELRIFLE_NPC_SPREAD + (6 - ent->NPC->currentAim) * 0.25f));
				angs[YAW] += (Q_flrand(-1.0f, 1.0f) * (REBELRIFLE_NPC_SPREAD + (6 - ent->NPC->currentAim) * 0.25f));
			}
			else
			{
				// add some slop to the alt-fire direction
				angs[PITCH] += Q_flrand(-1.0f, 1.0f) * REBELRIFLE_SPREAD;
				angs[YAW] += Q_flrand(-1.0f, 1.0f) * REBELRIFLE_SPREAD;
			}
		}

		AngleVectors(angs, dir, NULL, NULL);

		// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
		WP_RebelRifleMainFire(ent, dir);
	}
}