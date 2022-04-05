// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "ProjectileManager.generated.h"

//UENUM( BlueprintType )
//enum class Type:uint8
//{
//	AssaultRifle UMETA(DisplayName = "AssaultRifle"),
//	Pistol UMETA( DisplayName = "Pistol" ),
//	Fireball UMETA( DisplayName = "Fireball" )
//};


/***************************************************************************************
 * Type: Class
 *
 * Name: ProjectileManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose:
 *
 * Functions:
 *
 * References:
 *
 * See Also:
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 24/04/2022    JA			 1.0		 
 ***************************************************************************************/
UCLASS()
class PROJECTRELIC_API AProjectileManager : public AActor
{
	GENERATED_BODY()
private:
	bool isHit;
public:	
	/********************************************************
	   *   Function        :
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	// Sets default values for this actor's properties
	AProjectileManager();

protected:
	/********************************************************
	   *   Function        :
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/********************************************************
	   *   Function        :
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	// Called every frame
	virtual void Tick(float deltaTime) override;

	UPROPERTY( VisibleDefaultsOnly, Category = Projectile )
	USphereComponent* collisionComponent;

	// Projectile movement component
	UPROPERTY( VisibleAnywhere, Category = Movement )
	UProjectileMovementComponent* projectileMovementComponent;


	// Projectile Mesh
	UPROPERTY( VisibleDefaultsOnly, Category = Projectile )
		UStaticMeshComponent* projectileMeshComponent;

	// Projectile Mesh
	UPROPERTY( VisibleDefaultsOnly, Category = Movement )
		UMaterialInstanceDynamic* projectileMaterialInstance;
	
	
	/********************************************************
	   *   Function        :
	   *   Purpose         : When projectile hits something
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	UFUNCTION()
		void OnHit( UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit );
	/***********************************************************
	   *   Function        :
	   *   Purpose         : Initalise the projectile's velocity 
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	************************************************************/
	void ShootInDirection( const FVector& shootDirection );
};
