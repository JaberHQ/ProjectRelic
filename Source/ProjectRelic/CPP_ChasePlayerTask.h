// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CPP_AIController.h"
#include "CPP_ChasePlayerTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API UCPP_ChasePlayerTask : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory ) override;

};
