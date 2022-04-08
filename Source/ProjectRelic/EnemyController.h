// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyPatrolPoint.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyController.generated.h"
/***************************************************************************************
 * Type: Class
 *
 * Name: EnemyController
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Control the enemy patrol pawn and set its behaviour
 *
 * Functions: virtual void OnPossess( APawn* pawn ) override, AEnemyController(),
 *			  ~AEnemyController(), void SetPlayerCaught( APawn* pawn ), 
 *			  FORCEINLINE UBlackboardComponent* GetBlackboardComp() const,
 *			  FORCEINLINE TArray<AActor*> GetPatrolPoints() const
 *
 * References: Reubs via YouTube, https://www.youtube.com/watch?v=3Z1A825gTA8&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=4
 *				[Accessed: 20th March 2022 | Has been modified]
 *
 * See Also:
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA          1.0         Prototype, very basic pathing and vision
 ***************************************************************************************/
UCLASS()
class PROJECTRELIC_API AEnemyController : public AAIController
{
	GENERATED_BODY()

private:
	// Behaviour Tree comp
	UBehaviorTreeComponent* behaviourComp;

	// Blackboard Comp
	UBlackboardComponent* blackboardComp;

	// BB keys
	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName locationToGoKey;

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName playerKey;

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName hasLineOfSight;


	TArray<AActor*> patrolPoints;
	/**********************************************************************
	   *   Function        : virtual void OnPossess( APawn* pawn ) override
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         : 
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	***********************************************************************/
	virtual void OnPossess( APawn* pawn ) override;
public:
	/********************************************************
	   *   Function        : AEnemyController()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	AEnemyController();
	/********************************************************
	   *   Function        : ~AEnemyController()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	~AEnemyController();
	/***********************************************************
	   *   Function        : void SetPlayerCaught( APawn* pawn )
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	************************************************************/
	void SetPlayerCaught( const TArray<AActor*> CaughtActors );
	void SetHasLineOfSight( bool hasLineOfSight );
	bool GetHasLineOfSight();

	// Getters
	/***********************************************************************************
	   *   Function        : FORCEINLINE UBlackboardComponent* GetBlackboardComp() const
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	************************************************************************************/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const;
	/***********************************************************************************
	   *   Function        : FORCEINLINE TArray<AActor*> GetPatrolPoints() const
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	***********************************************************************************/
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const;
	
	int32 currentPatrolPoint = 0;
};
