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

protected:
	//UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
	//	class UAIPerceptionComponent* PerceptionComp;

	//UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
	//	class UAISenseConfig_Sight* SightConfig;


public:
	AEnemyController();
	~AEnemyController();

};
