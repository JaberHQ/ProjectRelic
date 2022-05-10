// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_PatrolPath.generated.h"
/**************************************************************************************
 * Type: Class
 *
 * Name: CPP_PatrolPath
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Patrol path of AI (Version 2 and above)
 * 
 * Functions: ACPP_PatrolPath(), virtual void BeginPlay() override,
 *			  virtual void Tick(float DeltaTime) override, 	
 *
 * References: N/A
 *
 * See Also: EnemyPatrolPoint.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 02/05/2022    JA          v2.0        Created an actor, needs further implementation
 **************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_PatrolPath : public AActor
{
	GENERATED_BODY()
private:
public:	
	// Sets default values for this actor's properties
	ACPP_PatrolPath();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
