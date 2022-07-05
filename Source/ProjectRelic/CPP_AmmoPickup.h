// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_Pickup.h"
#include "CPP_AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API ACPP_AmmoPickup : public ACPP_Pickup
{
	GENERATED_BODY()
private:
public:
	virtual void BeginPlay() override;

	virtual void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult ) override;
public:
	ACPP_AmmoPickup();
};
