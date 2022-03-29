// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTRELIC_API AEnemyController : public AAIController
{
	GENERATED_BODY()

	
private:
	void OnTargetPerceptionUpdated();

	//BT
	UBehaviorTreeComponent* BehaviourTreeComp;

	//BB
	UBlackboardComponent* BackboardComp;

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName PatrolLocationKey;

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName PlayerKey;

	// Store patrol points
	TArray<AActor*> PatrolPoints;
	
	virtual void OnPossess( APawn* Pawn ) override;
	

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UAIPerceptionComponent* PerceptionComp;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UAISenseConfig_Sight* SightConfig;

	

	

public:
	AEnemyController();
	~AEnemyController();

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const
	{
		return BackboardComp;
	}

	FORCEINLINE TArray<AActor*> GetPatrolPoints() const
	{
		return PatrolPoints;
	}



};
