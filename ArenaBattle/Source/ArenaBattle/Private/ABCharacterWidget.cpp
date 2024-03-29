// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacterWidget.h"
#include"ABCharacterStatComponent.h"
#include"Components/ProgressBar.h"

void UABCharacterWidget::BindCharacterStat(UABCharacterStatComponent *NewCharacterStat)
{
	
	ABCHECK(nullptr != NewCharacterStat);
	ABLOG(Warning, TEXT("In the Bind Character Stat"));

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddLambda([this]()->void {
		
		if (CurrentCharacterStat.IsValid())
		{
			ABLOG(Warning, TEXT("Your HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
		}
	});
	NewCharacterStat->OnHPChanged.AddUObject(this, &UABCharacterWidget::UpdateHPWidget);

}

void UABCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	ABCHECK(nullptr != HPProgressBar);

	UpdateHPWidget();
}

void UABCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}