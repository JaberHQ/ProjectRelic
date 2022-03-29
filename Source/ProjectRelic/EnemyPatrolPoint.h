// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EnemyPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API AEnemyPatrolPoint : public ATargetPoint
{
	GENERATED_BODY()
private:
protected:
public:
	AEnemyPatrolPoint();
	~AEnemyPatrolPoint();
};
