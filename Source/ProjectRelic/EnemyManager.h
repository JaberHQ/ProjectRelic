// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"
#include "EnemyManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API AEnemyManager : public ACharacterManager
{
	GENERATED_BODY()
public:
	AEnemyManager();
	~AEnemyManager();
	
	void UpdateWalkSpeed( float ChaseSpeed );
	UPROPERTY( EditAnywhere, Category = AI )
		class UBehaviorTree* BehaviourTree;

protected:
};
