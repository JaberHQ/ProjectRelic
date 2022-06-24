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
#include <Perception/AISenseConfig_Hearing.h>
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
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
 * 02/05/2022	 JA			 v2.0		 Redone EnemyCharacter.cpp
 * 17/05/2022    JA			 v2.1		 Added new patrol point features
 * 18/05/2022	 JA			 v2.2		 Multiple enemies can now follow different patrol points
 * 30/05/2022	 JA			 v2.3		 Added features for stealth takedown
 * 30/05/2022	 JA			 v2.4	     Added projectile features
 ***********************************************************************************************/

 // Declare delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSightDetectionD, float, detectionSpeed );

UCLASS()
class PROJECTRELIC_API ACPP_AIManager : public ACPP_CharacterManager
{
	GENERATED_BODY()
private:
	float m_detectionTimer; // detection timer
	float m_sightRadius; // Sight radius
	float m_loseSightRadius; // Lose sight radius
	float m_peripheralVisionAngleDegrees; // Peripheral vision
	float m_patrolSpeed; // Enemy walk (patrol) speed
	float m_chaseSpeed; // Enemy run (chase) speed
	float m_detectionSpeed; // Speed of detection
	bool m_canTakedown; // If AI can be taken down
	float m_shotDamage; // Damage per shot taken from Player
	float m_deathTimer; // Timer for despawn after death 

	float m_sightValuePercent;
	float m_detectionCount; // Counter for Stealth Detection UI 

	bool m_hasBeenSeen; // For when the player has been seen by AI, but not fully caught 
	bool m_hasBeenCaught; // For when the player has been caught by AI

	bool m_hasBeenShot;

	float m_curveFloat;
private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AI", meta = ( AllowPrivateAccess = "true" ) );
		ACPP_PatrolPoint* m_patrolPath; // Choose patrol points
public:
	UPROPERTY( EditAnywhere, Category = "AI" )
		class UBehaviorTree* behaviourTree; // Behaviour tree

	UPROPERTY( VisibleAnywhere, Category = "AI")
		class UAIPerceptionComponent* perceptionComp; // Perception Component

	UPROPERTY( VisibleAnywhere, Category = "AI" )
		class UAISenseConfig_Sight* sightConfig; // Sight configuration

	/*UPROPERTY( VisibleAnywhere, Category = "AI" )
		class UAISenseConfig_Hearing* hearingConfig;*/

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UBoxComponent* boxComponent; // Box Component

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animations" )
		UAnimMontage* animTakedownAI; // Animation Montage

	UPROPERTY( EditAnywhere, Category = "Health" )
		float health; // Player health

	UPROPERTY( EditAnywhere, Category = "Health" )
		float defaultHealth; // Player default health
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
	/*****************************************************************************
	 *   Function        : virtual void BeginPlay() override
	 *   Purpose         : BeginPlay event
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
   *****************************************************************************/
	virtual void BeginPlay() override;
	/*****************************************************************************
	 *   Function        : ACPP_AIManager()
	 *   Purpose         : Constructor
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
   *****************************************************************************/
	ACPP_AIManager();
	/*****************************************************************************
		 *   Function        : virtual void Tick( float DeltaTime ) override
		 *   Purpose         : Event Tick
		 *   Parameters      : float DeltaTime
		 *   Returns         : N/A
		 *   Date altered    : 02/05/2022
		 *   Contributors    : Jaber Ahmed
		 *   Notes           : N/A
		 *   See also        : N/A
		*****************************************************************************/
	virtual void Tick( float DeltaTime ) override;

public:
	/*****************************************************************************
	 *   Function        : ACPP_PatrolPoint* GetPatrolPath();
	 *   Purpose         : Get Patrol Points
	 *   Parameters      : N/A
	 *   Returns         : m_patrolPath
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : CPP_PatrolPoint
   *****************************************************************************/
	ACPP_PatrolPoint* GetPatrolPath();
	 /********************************************************************************************************************************************************************
	  *   Function        : void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, 
	  *							AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult  )
	  *   Purpose         : When Character overlaps (enters) the collision box
	  *   Parameters      : UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	  *							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	  *   Returns         : N/A
	  *   Date altered    : 30/05/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A           
	 *********************************************************************************************************************************************************************/
	UFUNCTION()
		void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
	/*****************************************************************************
	 *   Function        : void OnBoxEndOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
	 *   Purpose         : When Character leaves the collision box after overlap
	 *   Parameters      : UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
   *****************************************************************************/
	UFUNCTION()
		void OnBoxEndOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );
	/*****************************************************************************
	 *   Function        : void Takedown()
	 *   Purpose         : Action when AI has been stealth taken down
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : CPP_PlayerManager::Takedown
   *****************************************************************************/
	UFUNCTION()
		void Takedown();
	/*****************************************************************************
	 *   Function        : void DelayDeath()
	 *   Purpose         : After AI takedown, a delay before they are destroyed
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : CPP_AIManager::Takedown
   *****************************************************************************/
	void DelayDeath();
	/*****************************************************************************
	 *   Function        : virtual void TakeAttack() override
	 *   Purpose         : When AI has been hit by projectiles
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : Overriden for specific effect on relative actor
	 *   See also        : CPP_CharacterManager::ShootProjectile
   *****************************************************************************/
	virtual void TakeAttack() override;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = Curve )
		UCurveFloat* myCurve;

	UFUNCTION( BlueprintCallable )
		void SightDetectionDelegate();

	UPROPERTY( BlueprintCallable, BlueprintAssignable )
		FSightDetectionD sightDetectionD;

	void IncreaseSightDetectionIcon();

	void GiveUp();

	void SeenPlayer();

	void LostPlayer();
};
