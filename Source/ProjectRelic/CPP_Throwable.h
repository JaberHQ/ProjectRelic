// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CPP_Throwable.generated.h"

UCLASS()
class PROJECTRELIC_API ACPP_Throwable : public AActor
{
	GENERATED_BODY()
private:
	FVector m_velocity;
public:	
	// Sets default values for this actor's properties
	ACPP_Throwable();
	
	UPROPERTY( EditDefaultsOnly, Category = "Throwable" )
		class USkeletalMeshComponent* throwableMeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrowObject( FVector forwardVector );
};
