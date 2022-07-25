// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CPP_Throwable.generated.h"
/********************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_Throwable
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent class for throwable objects
 *
 * Functions: ACPP_Throwable, BeginPlay, Tick( float DeltaTime ), ThrowObject( FVector forwardVector )
 *
 * References: N/A
 *
 * See Also: CPP_PlayerManager
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 25/07/2022    JA			 v1.0        Created a throwable object
********************************************************************************************************************/
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
