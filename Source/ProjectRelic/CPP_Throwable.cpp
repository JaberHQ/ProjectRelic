// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Throwable.h"

// Sets default values
ACPP_Throwable::ACPP_Throwable()
	:m_forwardVectorMultiplier( 2500.0f )
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
	Super::Tick( DeltaTime );

}

void ACPP_Throwable::ThrowObject( FVector forwardVector )
{
	forwardVector *= m_forwardVectorMultiplier;

	// Detach and set physics
	throwableMeshComp->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform );
	throwableMeshComp->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	throwableMeshComp->SetSimulatePhysics( true );
	throwableMeshComp->SetPhysicsLinearVelocity( FVector::ZeroVector );

	// Add impulse
	throwableMeshComp->AddImpulse( forwardVector );
}

