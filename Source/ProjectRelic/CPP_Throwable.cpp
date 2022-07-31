// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Throwable.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPP_Throwable::ACPP_Throwable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	throwableMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "ThrowableMesh" ) );
}

// Called when the game starts or when spawned
void ACPP_Throwable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Throwable::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

}

void ACPP_Throwable::ThrowObject( FVector forwardVector, FVector playerLocation )
{
	forwardVector *= 2500.0f;

	throwableMeshComp->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
	throwableMeshComp->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	throwableMeshComp->SetSimulatePhysics( true );

	FVector direction = UKismetMathLibrary::GetDirectionUnitVector( playerLocation, GetActorLocation() );
	float speed = 800.0f;
	FVector vectorDirection = UKismetMathLibrary::Multiply_VectorFloat( direction, speed );
	throwableMeshComp->SetPhysicsLinearVelocity( vectorDirection );

	//throwableMeshComp->AddImpulse( forwardVector );
}

