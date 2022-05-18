// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "CPP_IncrementPointIndex.generated.h"

/************************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_IncrementPointIndex
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Increase or decrease the patrol point index
 *
 * Functions: UCPP_IncrementPointIndex( FObjectInitializer const& objectInitalizer ),
 *			  virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
 *
 * References: MrCxx, UE4 C++ AI Tutorial Series Part 5 Patrolling AI NPC
 *				 https://www.youtube.com/watch?v=99NT6lQ6SJ0&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=31
 *			     [Accessed 17/05/2022 | Modified]
 *
 * See Also: CPP_FindPatrolPathTask
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 17/05/2022    JA          v1.0	     Created Script
************************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API UCPP_IncrementPointIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	/*********************************************************************************************
	  *   Function        : UCPP_IncrementPointIndex( FObjectInitializer const& objectInitalizer )
	  *   Purpose         : Constructor
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 17/05/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A           
	*********************************************************************************************/
	UCPP_IncrementPointIndex( FObjectInitializer const& objectInitalizer );
	/*******************************************************************************************************************************
	*   Function        : virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override
	*   Purpose         : Execute task to find patrol points
	*   Parameters      : UBehaviorTreeComponent& ownerComp, uint8* nodeMemory
	*   Returns         : EBTNodeResult::Succeeded, EBTNodeResult::Failed
	*   Date altered    : 03/05/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*******************************************************************************************************************************/
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override;
private:
	/*****************************************************************************
	  *   Enum class      : EDirectionType
	  *   Purpose         : Change the direction of the AI
	  *   Enums			  : Forward, Backward
	  *   Date altered    : 17/05/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	*****************************************************************************/
	enum class EDirectionType
	{
		Forward, Reverse
	};

	EDirectionType m_direction; // Set direction
};
