// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CPP_Pickup.generated.h"
/*****************************************************************************************************
 * Type: Class
 *
 * Name: CPP_Pickup
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent for pickups
 *
 * References: N/A
 *
 * See Also: CPP_PlayerManager
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 25/07/2022    JA          v1.0		 Created pickup manager
*****************************************************************************************************/
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
	/******************************************************************************************************************************************************************
	 *   Function        : virtual void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, 
	 *						AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
	 *   Purpose         : Overlap object
	 *   Parameters      : PrimitiveComponent* OverlappedComp, 
	 *						AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	 *   Returns         : N/A
	 *   Date altered    :
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	******************************************************************************************************************************************************************/
	UFUNCTION()
	virtual void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

};
