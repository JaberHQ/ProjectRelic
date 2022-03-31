// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "EnemyPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyController.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTRELIC_API AEnemyController : public AAIController
{
	GENERATED_BODY()

private:
	// Behaviour Tree comp
	UBehaviorTreeComponent* BehaviourTreeComp;

	// Blackboard Comp
	UBlackboardComponent* BlackboardComp;

	// BB keys
	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName locationToGoKey;

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName playerKey;

	UPROPERTY( EditDefaultsOnly, Category = AI )
		TArray<AActor*> patrolPoints;

	virtual void OnPossess( APawn* Pawn ) override;

protected:
public:
	AEnemyController();
	~AEnemyController();
	
	void SetPlayerCaught( APawn* Pawn );

	// Getters
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const;
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const;
	
};
