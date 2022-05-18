// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterManager.h"
#include "NavigationSystem.h"
#include "CPP_PatrolPoint.h"
#include "CPP_AIManager.generated.h"

/***********************************************************************************************
 * Type: Class
 *
 * Name: CPP_AIManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent class for all AI
 *
 * Functions: ACPP_AIManager()
 *
 * References: N/A
 *
 * See Also: EnemyCharacter.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA			 v1.0        Pawn sense component
 * 09/04/2022	 JA			 v1.1		 Changed to AI Perception component		
 * 18/04/2022    JA			 v1.2		 Detection indicator
 * 25/04/2022	 JA			 v1.3		 Player to be invisible
 * 02/05/2022	 JA			 v2.0		 Redone EnemyCharacter
 * 17/05/2022    JA			 v2.1		 Added new patrol point features
 * 18/05/2022	 JA			 v2.2		 Multiple enemies can now follow different patrol points
 ***********************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_AIManager : public ACPP_CharacterManager
{
	GENERATED_BODY()
private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AI", meta = ( AllowPrivateAccess = "true" ) );
		ACPP_PatrolPoint* patrolPath; // Choose patrol points
public:
	UPROPERTY( EditAnywhere, Category = "AI" )
		class UBehaviorTree* behaviourTree; // Behaviour tree

public:

	ACPP_AIManager();
	ACPP_PatrolPoint* GetPatrolPath();
};
