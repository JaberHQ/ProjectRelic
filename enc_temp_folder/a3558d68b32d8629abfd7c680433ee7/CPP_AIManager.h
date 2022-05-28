// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterManager.h"
#include "NavigationSystem.h"
#include "CPP_PatrolPoint.h"
#include "Curves/CurveFloat.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include <UObject/ObjectMacros.h>
#include <Perception/AISenseConfig_Sight.h>
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
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
	float m_health; // health variable
	float m_detectionTimer; // detection timer
	float m_sightRadius; // Sight radius
	float m_loseSightRadius; // Lose sight radius
	float m_peripheralVisionAngleDegrees; // Peripheral vision
	float m_patrolSpeed; // Enemy walk (patrol) speed
	float m_chaseSpeed; // Enemy run (chase) speed
	float m_detectionSpeed; // Speed of detection
	bool m_canTakedown; // If enemy can be taken down
	bool m_hasBeenSeen; // Has seen the player

private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AI", meta = ( AllowPrivateAccess = "true" ) );
		ACPP_PatrolPoint* patrolPath; // Choose patrol points
public:
	UPROPERTY( EditAnywhere, Category = "AI" )
		class UBehaviorTree* behaviourTree; // Behaviour tree

	UPROPERTY( VisibleAnywhere, Category = "AI")
		class UAIPerceptionComponent* perceptionComp; // Perception Component

	UPROPERTY( VisibleAnywhere, Category = "AI" )
		class UAISenseConfig_Sight* sightConfig; // Sight configuration

	UPROPERTY( VisibleAnywhere, Category = "MeleeTakedown" )
		class UBoxComponent* boxComponent; // Box Component

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Animations )
		class UAnimMontage* animTakedown; // Animation Montage
private:
	/**********************************************************************************
	   *   Function        : void OnPlayerCaught( const TArray<AActor*>& CaughtActors )
	   *   Purpose         : What to do when enemy has seen an actor
	   *   Parameters      : const TArray<AActor*>& CaughtActors
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : EnemyController
	**********************************************************************************/
	UFUNCTION()
		void OnPlayerCaught( const TArray<AActor*>& caughtActors );
public:
	virtual void BeginPlay() override;
	ACPP_AIManager();
	ACPP_PatrolPoint* GetPatrolPath();

	UFUNCTION()
		void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult  );

	UFUNCTION()
		void OnBoxEndOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

	UFUNCTION()
		void Takedown();
};
