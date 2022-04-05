// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
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
 * Functions: OnPlayerCaught( APawn* pawn ), AEnemyCharacter(), ~AEnemyCharacter(), 
 *			  UpdateWalkSpeed( float chaseSpeed ), void BeginPlay() override, 
 *			  AEnemyCharacter* GetEnemyCharacter( APawn* pawn ) const
 *
 * References:
 *
 * See Also:
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA			 1.0     ----------------------------------------------
 ***************************************************************************************/
UCLASS()
class PROJECTRELIC_API AEnemyCharacter : public ACharacterManager
{
	GENERATED_BODY()
	
private:
	// Enemy health variable
	float m_health;

	/**********************************************************
	   *   Function        : void OnPlayerCaught( APawn* pawn )
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	**********************************************************/
	UFUNCTION()
		void OnPlayerCaught( APawn* pawn );
public:
	/********************************************************
	   *   Function        : AEnemyCharacter()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	AEnemyCharacter();
	/********************************************************
	   *   Function        : ~AEnemyCharacter()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	~AEnemyCharacter();
	/****************************************************************
	   *   Function        : void UpdateWalkSpeed( float chaseSpeed )
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*****************************************************************/
	void UpdateWalkSpeed( float chaseSpeed );
	/********************************************************
	   *   Function        : void BeginPlay() override
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	void BeginPlay() override;

	UPROPERTY( EditAnywhere, Category = AI )
		class UBehaviorTree* behaviourTree;

	UPROPERTY( VisibleAnywhere, Category = AI )
		class UPawnSensingComponent* pawnSensingComp;
	/*******************************************************************************************
	   *   Function        : FORCEINLINE AEnemyCharacter* GetEnemyCharacter( APawn* pawn ) const
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*******************************************************************************************/
	FORCEINLINE AEnemyCharacter* GetEnemyCharacter( APawn* pawn ) const;
};
