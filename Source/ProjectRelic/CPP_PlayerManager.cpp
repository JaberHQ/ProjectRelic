// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ACPP_PlayerManager::ACPP_PlayerManager()
	:m_canTakedown( true )
	,m_takedownTraceDistance( 2000.0f )
{
}

void ACPP_PlayerManager::BeginPlay()
{
	Super::BeginPlay();

	// Start player off as crouching
	Crouch();
}

void ACPP_PlayerManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAction( "MeleeTakedown", IE_Pressed, this, &ACPP_PlayerManager::Takedown );
}

void ACPP_PlayerManager::SetCanTakedown( bool canTakedown )
{
	
	m_canTakedown = canTakedown;
	
}

bool ACPP_PlayerManager::GetCanTakedown()
{
	return m_canTakedown;
}

void ACPP_PlayerManager::Takedown()
{
	if( m_canTakedown )
	{
		TraceForward_Implementation();
	}
}

void ACPP_PlayerManager::TraceForward_Implementation()
{
	FVector location;
	FRotator rotation;
	FHitResult hit;
	FCollisionQueryParams traceParams;

	GetController()->GetPlayerViewPoint( location, rotation );

	FVector start = location;
	FVector end = ( start + ( rotation.Vector() * m_takedownTraceDistance ) );


	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECC_Camera, traceParams );

	DrawDebugLine( GetWorld(), start, end, FColor::Red, false, 5.0f );

	if( bHit )
	{
		DrawDebugBox( GetWorld(), hit.ImpactPoint, FVector( 5, 5, 5 ), FColor::Emerald, false, 2.0f );
	}
}
