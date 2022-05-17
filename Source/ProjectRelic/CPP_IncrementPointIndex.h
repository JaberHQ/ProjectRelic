// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "CPP_IncrementPointIndex.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API UCPP_IncrementPointIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
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
	enum class EDirectionType
	{
		Forward, Reverse
	};

	EDirectionType direction = EDirectionType::Forward;
};
