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
 * Name: CPP_AIController
 *
 * Author: Jaber Ahmed
 *
 * Purpose: AI Controller to control Enemy Behaviour
 *
 * Functions: ACPP_AIController(), UBlackboardComponent* GetBlackboardComp() const
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 02/05/2022   JA           v2.0        Create a better EnemyController
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

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName patrolPathVector; // Patrol path vector

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName patrolPathIndex; // Patrol path index

public:
	/*****************************************************************************
	*   Function        : ACPP_AIController()
	*   Purpose         : Constructor
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 02/05/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	ACPP_AIController();
	/**********************************************************************************
	*   Function        : FORCEINLINE UBlackboardComponent* GetBlackboardComp() const
	*   Purpose         : Get Blackboard component
	*   Parameters      : N/A
	*   Returns         : blackboardComp
	*   Date altered    : 02/05/2022 
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	**********************************************************************************/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const;
};
