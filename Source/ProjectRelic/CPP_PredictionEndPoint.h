// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CPP_PredictionEndPoint.generated.h"
/********************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_PredictionEndPoint
 *
 * Author: Jaber Ahmed
 *
 * Purpose: End point actor for the end of the throwing prediction
 *
 * Functions: ACPP_PredictionEndPoint(), UStaticMeshComponent* endPoint, BeginPlay(), Tick( float DeltaTime )
 *
 * References: N/A
 *
 * See Also: CPP_PlayerManager
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 25/07/2022    JA			 v1.0        Created a prediction end point
********************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_PredictionEndPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_PredictionEndPoint();

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		class UStaticMeshComponent* endPoint;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
