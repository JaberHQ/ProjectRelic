// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyController.h"

//#include "Engine.h".

AEnemyCharacter::AEnemyCharacter()
	:m_health( 100.0f )
	,m_detectionTimer( 0.0f )
	//,m_timerHandle()
	//,m_RepeatingCallsRemaining(10)
{
	//Initialise senses
	/*pawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>( TEXT( "PawnSensingComp" ) );
	pawnSensingComp->SetPeripheralVisionAngle( 90.0f );*/

	// Set default walk speed
	UpdateWalkSpeed( 300.0f );

	perceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "AIPerception Component" ) );
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "Sight Config" ) );
	perceptionComp->ConfigureSense( *sightConfig );
	perceptionComp->SetDominantSense( sightConfig->GetSenseImplementation() );

	sightConfig->SightRadius = 1000.0f;
	sightConfig->LoseSightRadius = 3020.0f;
	sightConfig->PeripheralVisionAngleDegrees = 360.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

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

	perceptionComp->OnPerceptionUpdated.AddDynamic( this, &AEnemyCharacter::OnPlayerCaught );

	//if( pawnSensingComp )
	//{
	//	// If enemy 'senses' player
	//	pawnSensingComp->OnSeePawn.AddDynamic( this, &AEnemyCharacter::OnPlayerCaught );
	//}
}

AEnemyCharacter* AEnemyCharacter::GetEnemyCharacter( APawn* pawn ) const
{
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>( pawn );
	return enemyCharacter;
}



void AEnemyCharacter::OnPlayerCaught( const TArray<AActor*>& CaughtActors )
{
	// Get reference to player controller
	AEnemyController* enemyController = Cast<AEnemyController>( GetController() );
	
	enemyController->SetHasLineOfSight( true );
	
	if( enemyController )
	{	
		// Debug message
		GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "Pew" ) ) );
			//enemyController->SetPlayerCaught( CaughtActors );
		UAIPerceptionSystem::RegisterPerceptionStimuliSource( this, sightConfig->GetSenseImplementation(), enemyController );
			// Shoot at player
		Shoot();

			//UpdateWalkSpeed( 600.0f );				
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


	//if( pawnSensingComp->SightRadius >= GetDistanceTo( pawn ) )
		//{
		//	// Set has line of sight = false
		//	enemyController->SetHasLineOfSight( true );
		//}
		//else
		//{
		//	enemyController->SetHasLineOfSight( false );
		//}