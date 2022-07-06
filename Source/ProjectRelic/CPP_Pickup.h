// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CPP_Pickup.generated.h"

UCLASS()
class PROJECTRELIC_API ACPP_Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Pickup();

private:
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UBoxComponent* boxComp;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UStaticMeshComponent* cubeComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

};
