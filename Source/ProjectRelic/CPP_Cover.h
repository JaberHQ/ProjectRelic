// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Cover.generated.h"
/*****************************************************************************************************
 * Type: Class
 *
 * Name: CPP_Cover
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Made for cover system
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 04/08/2022    JA          N/A         DEPRECIATED
*****************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_Cover : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Cover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
