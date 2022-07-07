// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Pickup.h"
#include <Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h>

// Sets default values
ACPP_Pickup::ACPP_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>( TEXT( "Box" ) );
	cubeComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Cube" ) );
	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>( TEXT( "StaticMesh'/Engine/BasicShapes/Cube.Cube'" ) ).Object;
	cubeComp->SetStaticMesh( cubeMesh );
	//cubeComp->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap );
}

void ACPP_Pickup::OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	Destroy();
}

// Called when the game starts or when spawned
void ACPP_Pickup::BeginPlay()
{
	Super::BeginPlay();
	

	boxComp->AttachToComponent( cubeComp, FAttachmentTransformRules::SnapToTargetIncludingScale );
	boxComp->OnComponentBeginOverlap.AddDynamic( this, &ACPP_Pickup::OnBoxBeginOverlap );

	RootComponent = cubeComp;

}

// Called every frame
void ACPP_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

