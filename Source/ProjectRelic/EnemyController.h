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
	float m_lineOfSightTimer;
	FTimerHandle m_enemyTimer;
	FName m_enemyActor;
	FName m_hasLineOfSight;
protected:
public:
	AEnemyController();
	~AEnemyController();
	void onTargetPerceptionUpdated();
};
