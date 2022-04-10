// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include <UObject/ObjectMacros.h>
#include <Perception/AISenseConfig_Sight.h>
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
UCLASS()
class PROJECTRELIC_API AEnemyCharacter : public ACharacterManager
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
};
