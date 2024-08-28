
const functions = require('firebase-functions');
const admin = require('firebase-admin');
const { google } = require('googleapis');
const OAuth2 = google.auth.OAuth2;

// Initialize Firebase Admin
admin.initializeApp();

// Google OAuth2 Client setup
const oauth2Client = new OAuth2(
  '1033517959127-v0fmrjj0qogd8sc82evunhvlilqjqn43.apps.googleusercontent.com',
  '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',
  'https://accounts.google.com/o/oauth2/auth'
);

// Set credentials (including the refresh token)
oauth2Client.setCredentials({
  refresh_token: 'YOUR_REFRESH_TOKEN' // Replace with your actual refresh token
});

// Google Calendar API setup
const calendar = google.calendar('v3');

// Function to update calendar events
async function updateCalendarEvents() {
  try {
    const now = new Date();
    const events = await calendar.events.list({
      calendarId: 'primary',
      timeMin: now.toISOString(),
      timeMax: new Date(now.getTime() + 24 * 60 * 60 * 1000).toISOString(),
      singleEvents: true,
      orderBy: 'startTime',
      auth: oauth2Client
    });

    const eventList = events.data.items.map(event => ({
      id: event.id,
      summary: event.summary,
      start: event.start.dateTime || event.start.date,
      end: event.end.dateTime || event.end.date
    }));

    await admin.database().ref('calendar/events').set(eventList);
    console.log('Calendar events updated successfully');
  } catch (error) {
    console.error('Error updating calendar events:', error);
  }
}

// HTTP trigger to update calendar events
exports.triggerUpdateCalendarEvents = functions.https.onRequest(async (req, res) => {
  await updateCalendarEvents();
  res.send('Calendar events updated manually.');
});

// Scheduled trigger to update calendar events every 24 hours
exports.updateCalendarEvents = functions.pubsub.schedule('every 24 hours').onRun(async (context) => {
  await updateCalendarEvents();
});
