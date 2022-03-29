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
<<<<<<< HEAD
	~AEnemyManager();
	
	void UpdateWalkSpeed( float ChaseSpeed );

protected:
=======
>>>>>>> parent of b521974 (WIP Enemy AI C++)
};
