// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API AEnemyCharacter : public ACharacterManager
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();
	~AEnemyCharacter();
	void UpdateWalkSpeed( float ChaseSpeed );
	UPROPERTY( EditAnywhere, Category = AI )
		class UBehaviorTree* BehaviourTree;
protected:
private:

};
