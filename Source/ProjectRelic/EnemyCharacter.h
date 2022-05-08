// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"
#include "Curves/CurveFloat.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include <UObject/ObjectMacros.h>
#include <Perception/AISenseConfig_Sight.h>
#include "CombatInterface.h"
#include "EnemyCharacter.generated.h"

/***************************************************************************************
 * Type: Class
 *
 * Name: EnemyCharacter
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Child class for enemy patrol
 *
 * Functions: OnPlayerCaught( const TArray<AActor*>& CaughtActors ), AEnemyCharacter(),
 *			  ~AEnemyCharacter(), UpdateWalkSpeed( float chaseSpeed ), 
 *			  void BeginPlay() override, 
 *			  AEnemyCharacter* GetEnemyCharacter( APawn* pawn ) const
 *
 * References: N/A
 *
 * See Also: EnemyController, EnemyPatrolPoint, SelectEnemyPatrolPoint, ProjectileManager
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA			 1.0         Pawn sense component
 * 09/04/2022	 JA			 1.1		 AI Perception component			
 ***************************************************************************************/

// Declare delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FSightRegisteredD, bool, hasBeenSeen, float, detectionSpeed );

UCLASS()
class PROJECTRELIC_API AEnemyCharacter : public ACharacterManager, public ICombatInterface
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
	
	bool m_canTakedown; // If enemy can be taken down

	bool m_hasBeenSeen; // Has seen the player
	float m_detectionSpeed; // Speed of detection


	// Reference to interface
	//TUniquePtr<ICombatInterface> m_pCombatInterface = MakeUnique<ICombatInterface>();
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

	/********************************************************
	   *   Function        : AEnemyCharacter()
	   *   Purpose         : Constructor
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	AEnemyCharacter();
	/********************************************************
	   *   Function        : ~AEnemyCharacter()
	   *   Purpose         : Deconstructor
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	~AEnemyCharacter();
	/********************************************************
	   *   Function        : void BeginPlay() override
	   *   Purpose         : On start of play
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void BeginPlay() override;

	UPROPERTY( EditAnywhere, Category = AI )
		class UBehaviorTree* behaviourTree;

	UPROPERTY( VisibleAnywhere, Category = AI )
		class UAIPerceptionComponent* perceptionComp;

	UPROPERTY( VisibleAnywhere, Category = AI )
		class UAISenseConfig_Sight* sightConfig;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UserInterface )
		class UWidgetComponent* widgetComp;
	
	UPROPERTY( BlueprintCallable, BlueprintAssignable )
		FSightRegisteredD sightRegisteredD;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = Curve )
		UCurveFloat* myCurve;
	/*******************************************************************************************
	   *   Function        : FORCEINLINE AEnemyCharacter* GetEnemyCharacter( APawn* pawn ) const
	   *   Purpose         : Getter for the enemy character
	   *   Parameters      : APawn* pawn
	   *   Returns         : enemyCharacter
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*******************************************************************************************/
	FORCEINLINE AEnemyCharacter* GetEnemyCharacter( APawn* pawn ) const;
	/*****************************************************************************
	*   Function        : bool CanTakedown() 
	*   Purpose         : Get bool to check if enemy can be taken down
	*   Parameters      : N/A
	*   Returns         : m_canTakedown
	*   Date altered    : 18/04/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	bool CanTakedown();
	/*****************************************************************************
	*   Function        : void Interact() 
	*   Purpose         : Function for when Player and Enemy interact
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 18/04/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void Interact();
	/*****************************************************************************
	*   Function        : void SightDetectionDelegate()
	*   Purpose         : Event for when player has been detected
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 20/04/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	UFUNCTION( BlueprintCallable )
		void SightDetectionDelegate();
	/*****************************************************************************
	*   Function        : void SetHasBeenSeen( bool hasBeenSeen )
	*   Purpose         : Set bool
	*   Parameters      : bool hasBeenSeen 
	*   Returns         : N/A
	*   Date altered    : 20/04/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void SetHasBeenSeen( bool hasBeenSeen );
	/*****************************************************************************
	*   Function        : bool GetHasBeenSeen() const
	*   Purpose         : Get bool
	*   Parameters      : N/A
	*   Returns         : return m_hasBeenSeen
	*   Date altered    : 20/04/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	bool GetHasBeenSeen() const;
	/*****************************************************************************
	*   Function        : float GetDetectionSpeed() const
	*   Purpose         : Get the speed of detection
	*   Parameters      : N/A
	*   Returns         : return m_detectionSpeed
	*   Date altered    : 21/04/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	float GetDetectionSpeed() const;
};
