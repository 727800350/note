var events=CalendarApp.getEventsForDay(day);
  for(i=0;i<events.length;i++){
    var event=events[i];
    doc.appendParagraph('------------------------------------------------The Next Event--------------------------------------------------');
    doc.appendParagraph('title:'+event.getTitle());
    doc.appendParagraph('id:'+event.getId());
    doc.appendParagraph('description:'+event.getDescription());
}
