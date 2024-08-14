#include "HUDManager.h"

void UHUDManager::Push(UUserWidget *widget)
{
    if(widget)
    {
        widget->AddToViewport();
        WidgetStack.Add(widget);
    }
}

UUserWidget *UHUDManager::Pop()
{
    if(WidgetStack.Num() > 0)
    {
        UUserWidget *widget = WidgetStack.Last();
        
        WidgetStack.RemoveAt(WidgetStack.Num() - 1);
        return widget;
    }
    return nullptr;
}

UUserWidget *UHUDManager::Peek() const
{
    if(WidgetStack.Num() > 0)
    {
        return WidgetStack.Last();
    }

    return nullptr;
}

