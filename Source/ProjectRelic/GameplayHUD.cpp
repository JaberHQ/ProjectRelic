// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayHUD.h"

void AGameplayHUD::DrawHUD()
{
	Super::DrawHUD();

	if( CrosshairTexture )
	{
		// Centre of canvas
		FVector2D Centre( Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f );

		// Offset by half of texture
		FVector2D CrossHairDrawPosition( Centre.X - ( CrosshairTexture->GetSurfaceWidth() * 0.5f ), Centre.Y - ( CrosshairTexture->GetSurfaceHeight() * 0.5f ) ) ;

		// Draw crosshair
		FCanvasTileItem TileItem( CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White );
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem( TileItem );
	}
}
