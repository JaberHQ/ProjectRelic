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
 * References: https://www.youtube.com/watch?v=3IDflM4GuCY&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=9
 *				https://www.youtube.com/watch?v=nshHCycft4A&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=9
 *				https://www.youtube.com/watch?v=owZz-ffUzsc&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=11
 *				https://www.youtube.com/watch?v=iQ1QsqcWFMI&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=12
 *				https://www.youtube.com/watch?v=0XbM0O-gwxU&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=13
 *				https://www.youtube.com/watch?v=0gU5StGSq_o&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=15
 *				https://www.youtube.com/watch?v=nn057V9KQDs&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=19
 *				https://www.youtube.com/watch?v=O5o_usc2pWo&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=22
 *				https://www.youtube.com/watch?v=0ZvAnUTodfI&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=25
 *				https://www.youtube.com/watch?v=WDzIk7dP4d4&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=30
 *				https://www.youtube.com/watch?v=99NT6lQ6SJ0&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=31
 *				https://www.youtube.com/watch?v=GdLvZacVmkA&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=39
 *				https://www.youtube.com/watch?v=aACvp7BBeCk&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=42
 *				
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
 * 13/06/2022	 JA			 v2.5		 Detection Meter
 * 20/06/2022	 JA			 v2.6		 Further AI tweaking
 * 04/07/2022	 JA			 v2.7		 Invisibility
 ***********************************************************************************************/

 // Declare delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FSightDetectionD, float, detectionSpeed, bool, hasSeenSomething );

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
	float m_shotDamage; // Damage per shot taken from Player
	float m_deathTimer; // Timer for despawn after death 
	float m_headShotDamage; // Damage multiplier for headshots
	float m_sightValuePercent; // Sight value percent
	float m_curveFloat; // Value for sight detection curve
	bool m_canTakedown; // If AI can be taken down
	bool m_hasBeenSeen; // For when the player has been seen by AI, but not fully caught 
	bool m_hasBeenCaught; // For when the player has been caught by AI
	bool m_hasBeenShot; // AI has been shot
	bool m_hasSeenSomething; // If AI has seen but no caught something
	bool m_isInCover; // AI is in cover
	bool m_dead; // If AI is dead

	FTimerHandle m_stopShooting;

private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AI", meta = ( AllowPrivateAccess = "true" ) );
		ACPP_PatrolPoint* m_patrolPath; // Choose patrol points

	UPROPERTY( EditAnywhere, Category = "SFX" )
		USoundBase* soundHuh; // Sound huh -- Depreciated since the BT has sound cue's, better to use those

public:
	UPROPERTY( EditAnywhere, Category = "AI" )
		class UBehaviorTree* behaviourTree; // Behaviour tree

	UPROPERTY( VisibleAnywhere, Category = "AI")
		class UAIPerceptionComponent* perceptionComp; // Perception Component

	UPROPERTY( EditAnywhere, Category = "AI" )
		class UAISenseConfig_Sight* sightConfig; // Sight configuration

	UPROPERTY( EditAnywhere, Category = "AI" )
		class UAISenseConfig_Hearing* hearingConfig; // Hearing configuration

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UBoxComponent* boxComponent; // Box Component

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animations" )
		UAnimMontage* animTakedownAI; // Animation Montage

	UPROPERTY( EditAnywhere, Category = "Health" )
		float health; // Player health

	UPROPERTY( EditAnywhere, Category = "Health" )
		float defaultHealth; // Player default health

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animations" )
		UAnimMontage* animDeath; // Animation Montage

	UPROPERTY( BlueprintCallable, BlueprintAssignable )
		FSightDetectionD sightDetectionD; // Sight detection delegate

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Curve" )
		UCurveFloat* myCurve; // Curve for sight detection

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Noise" )
		FName bottleTag; // Tag for for bottle noise
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
	 *   Function        : bool GetHasCaughtPlayer()
	 *   Purpose         : Get If player has been caught by AI
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 28/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetHasCaughtPlayer();
	/*****************************************************************************
	 *   Function        : void OnUpdated( const TArray<AActor*> & caughtActors )
	 *   Purpose         : What to do when player has been caught
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 28/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	UFUNCTION( BlueprintCallable )
		void OnUpdated( const TArray<AActor*>& caughtActors );
	/*****************************************************************************
  *   Function        : void GiveUp()
  *   Purpose         : When AI loses the player
  *   Parameters      : N/A
  *   Returns         : N/A
  *   Date altered    : 01/07/2022
  *   Contributors    : Jaber Ahmed
  *   Notes           : N/A
  *   See also        : N/A
  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		void GiveUp();
	/****************************************************************************************
	*   Function        : void SightDetectionDelegate();
	*   Purpose         : Delegate macro to send variables to BP for sight detection meter
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 01/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : Uses macro that sends two params
	*   See also        : N/A
	**************************************************************************************/
	UFUNCTION( BlueprintCallable )
		void SightDetectionDelegate();
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
	*   Function        : bool GetInCover()
	*   Purpose         : Get bool for AI in cover
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 02/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
   *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetInCover();
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
	/*****************************************************************************
	  *   Function        : void EvaluateSightDetection();
	  *   Purpose         : Whether sight detection meter goes up or down
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void EvaluateSightDetection();
	/*****************************************************************************
	  *   Function        : void SeenPlayer()
	  *   Purpose         : What AI do when they see the player
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void SeenPlayer();
	/*****************************************************************************
	  *   Function        : void LostPlayer()
	  *   Purpose         : What AI to do when they lose the player
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void LostPlayer();
	/*****************************************************************************
	  *   Function        : float DetectionSpeedCalculation()
	  *   Purpose         : Calculation for stealth detection widget
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	float DetectionSpeedCalculation();
	  /*****************************************************************************
	*   Function        : void DelayInvestigate()
	*   Purpose         : Delay AI investigating state
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 01/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void DelayInvestigate();
	/*****************************************************************************
	 *   Function        : void SetupPerceptionSystem()
	 *   Purpose         : Setup the sight perception component
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 28/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void SetupPerceptionSystem();
	/*****************************************************************************
	 *   Function        : void EnterCover()
	 *   Purpose         : AI action when entering cover 
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 28/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : CPP_EnterCoverTask
	*****************************************************************************/
	void EnterCover();
	/*****************************************************************************
	 *   Function        : void TimeToShoot();
	 *   Purpose         : Shooting task when in cover
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 28/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : CPP_ShootFromCover
	*****************************************************************************/
	void TimeToShoot();
	/*****************************************************************************
	 *   Function        : void SetHasCaughtPlayer( bool boolean )
	 *   Purpose         : Set boolean to catch the player
	 *   Parameters      : bool boolean 
	 *   Returns         : N/A
	 *   Date altered    : 28/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void SetHasCaughtPlayer( bool boolean );
	/*********************************************************************************
	 *   Function        : void SetMaxWalkSpeed( float speed )
	 *   Purpose         : Set max walk speed of the AI
	 *   Parameters      : float speed
	 *   Returns         : N/A
	 *   Date altered    : 28/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : Possibly unnecessary since character manager also has this
	 *   See also        : N/A
	**********************************************************************************/
	void SetMaxWalkSpeed( float speed );
	/***********************************************************************************
	 *   Function        : void SetSightValuePercent( float sightValuePercent )
	 *   Purpose         : Set sight value percent
	 *   Parameters      : float sightValuePercent 
	 *   Returns         : N/A
	 *   Date altered    : 28/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : Created to make sight value full when player has been caught
	 *   See also        : N/A
	***********************************************************************************/
	void SetSightValuePercent( float sightValuePercent );
	/***********************************************************************************
	 *   Function        : bool GetCanTakedown();
	 *   Purpose         : Get can takedowb
	 *   Parameters      : N/A
	 *   Returns         : m_canTakedown
	 *   Date altered    : 28/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : CPP_PLayerManager::Takedown
	***********************************************************************************/
	bool GetCanTakedown();

	void ShootPlayer();
	void StopShootingPlayer();
};
