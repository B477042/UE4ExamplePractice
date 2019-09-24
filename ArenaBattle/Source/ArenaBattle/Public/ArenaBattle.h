// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.


#pragma once

#include "EngineMinimal.h"

/*
chapter 14 Create Statue Machine

PREINIT : Statue that Before Creating Character. 
				Asset has Setted Not Loaded.
				Hide From World ; UI and Character
				Can not Take Damage

LOADING : Load Selected Character. 
				Decide AI control And Player Control
				Asset be Load
				Can't Control Character while Asset Loading Complete

READY : Asset Loaded
			Show Character and UI
			Can Take Damage
			IsPlayer-> Player Control
			!IsPlayer-> BehaviorTree Control

DEAD : HP <=0
			Play Dead Animation
			Turn off UI, Delete Collison, Stop Behavior Tree if is AI
			Some time had passed, If Player-> Restart.
			Nor Get AI off World

ECharacterState -> Can used at Blueprint
*/
UENUM(BlueprintType)
enum class ECharacterState :uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};


DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define ABCHECK(Expr, ...){if(!(Expr)){ABLOG(Error,TEXT("ASSERTION : %s"),TEXT("'"#Expr"'")); return __VA_ARGS__;}}