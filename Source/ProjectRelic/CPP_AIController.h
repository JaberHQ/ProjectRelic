// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "CPP_AIController.generated.h"
/**************************************************************************************
 * Type: Class
 *
 * Name:
 *
 * Author: Jaber Ahmed
 *
 * Purpose:
 *
 * Functions:
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * ----------    JA          -------     ----------------------------------------------
 **************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_AIController : public AAIController
{
	GENERATED_BODY()
	

private:
	UBehaviorTreeComponent* behaviourComp; // Behaviour Tree comp
	UBlackboardComponent* blackboardComp; // Blackboard Comp

	// BB keys
	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName patrolLocation; // Patrol location

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName enemyActor; // Target

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName hasLineOfSight; // Has line of sight bool
public:
	ACPP_AIController();
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const;
};
