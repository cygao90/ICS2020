#include <am.h>
#include <nemu.h>

uint32_t st_time = 0;

void __am_timer_init() {
  st_time = inl(RTC_ADDR);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = (inl(RTC_ADDR) - st_time);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
