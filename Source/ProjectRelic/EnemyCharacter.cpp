// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyController.h"

//#include "Engine.h".

AEnemyCharacter::AEnemyCharacter()
{
	//Initialise senses
	pawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>( TEXT( "PawnSensingComp" ) );
	pawnSensingComp->SetPeripheralVisionAngle( 90.0f );

	// Default enemy health
	m_health = 100.0f;

	UpdateWalkSpeed( 300.0f );

	detectionTimer = 0.0f;

	// Set sight raidus
	pawnSensingComp->SightRadius = 1000.0f;
}

AEnemyCharacter::~AEnemyCharacter()
{
}

void AEnemyCharacter::UpdateWalkSpeed( float chaseSpeed )
{
	// Set max walk speed
	GetCharacterMovement()->MaxWalkSpeed = chaseSpeed;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if( pawnSensingComp )
	{
		// If enemy 'senses' player
		pawnSensingComp->OnSeePawn.AddDynamic( this, &AEnemyCharacter::OnPlayerCaught );
	}
}

AEnemyCharacter* AEnemyCharacter::GetEnemyCharacter( APawn* pawn ) const
{
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>( pawn );
	return enemyCharacter;
}

void AEnemyCharacter::OnPlayerCaught( APawn* pawn )
{
	// Get reference to player controller
	AEnemyController* enemyController = Cast<AEnemyController>( GetController() );

	if( pawnSensingComp->SightRadius > GetDistanceTo( pawn ) )
	{
		// Set has line of sight = false
		enemyController->SetHasLineOfSight( true );
	}
	else
	{
		enemyController->SetHasLineOfSight( false );
	}
	
	if( enemyController )
	{
		if( enemyController->GetHasLineOfSight() == true )
		{
			// Debug message
			GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "Pew" ) ) );

			// Set bool
			enemyController->SetPlayerCaught( pawn );

			// Shoot at player
			Shoot();
		}
	}

}



/*if( enemycontroller->GetPlayerCaught() == false && detectionTimer > 0 )
	{
		detectionTimer -= 1.0f;
	}*/

	/*if( detectionTimer < 5.0f )
	{
		detectionTimer += 1.0f;
		FString timerString = FString::SanitizeFloat( detectionTimer );
		GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, *timerString );
	}*/

	/*if( detectionTimer >= 5.0f )
	{
	}*/