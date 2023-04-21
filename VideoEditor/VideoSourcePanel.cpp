#include "VideoSourcePanel.h"
#include <libswscale/swscale.h>
#undef av_err2str
//extern char x[AV_ERROR_MAX_STRING_SIZE];
#define av_err2str(errnum) \
    av_make_error_string(x, AV_ERROR_MAX_STRING_SIZE, errnum)

wxDEFINE_EVENT(SHOW_VIDEO_EVENT, wxCommandEvent);
wxDEFINE_EVENT(WIDGET_DELETED_EVENT, wxCommandEvent);
VideoSourcePanel::VideoSourcePanel(wxWindow* parent, VideoSource* videoSource, wxWindowID showWindow) : wxPanel(parent, wxID_ANY), m_videoSource(videoSource)
{
	m_showWindowID = showWindow;
	const AVFrame* frame = m_videoSource->getFirstFrame();
	AVFrame* rgbFrame = av_frame_alloc(); // a copy frame of the first frame in the videoSource
	 // set the properties of the copy to match the og frame
	rgbFrame->width = frame->width;
	rgbFrame->height = frame->height;
	rgbFrame->format = AV_PIX_FMT_RGB24;

	// allocate memofy for the copy
	int ret = av_frame_get_buffer(rgbFrame, 0);
	if (ret < 0) {
		wxMessageBox((std::string)"ERROR: Allocat memory for VideoSourcePanel: " +av_err2str(ret));
	}

	// Set up the SwsContext* for the conversion
	SwsContext* swsContext = sws_getContext(
		frame->width, frame->height, (AVPixelFormat)frame->format,
		rgbFrame->width, rgbFrame->height, AV_PIX_FMT_RGB24,
		SWS_BILINEAR, NULL, NULL, NULL
	);

	// Convert the frame from YUV to RGB
	sws_scale(swsContext, frame->data, frame->linesize, 0,
		frame->height, rgbFrame->data, rgbFrame->linesize);
	// Clean up
	sws_freeContext(swsContext);

	wxImage image(rgbFrame->width, rgbFrame->height, rgbFrame->data[0], true);
	wxSize thumbnailSize(120, 84);
	wxImage finalImg(thumbnailSize);
	finalImg.SetRGB(wxRect(thumbnailSize), 0, 0, 0);
	int w = image.GetWidth();
	int h = image.GetHeight();
	double scaleFactor = 1.0;
	if (w > h) {
		scaleFactor = 1.0 * thumbnailSize.GetWidth() / w;
	}
	else {
		scaleFactor = 1.0 * thumbnailSize.GetHeight() / h;
	}
	int scaledW = w * scaleFactor;
	int scaledH = h * scaleFactor;
	image.Rescale(scaledW, scaledH, wxIMAGE_QUALITY_HIGH);
	finalImg.Paste(image, (thumbnailSize.GetWidth() - scaledW / 2 - thumbnailSize.GetWidth() / 2), (thumbnailSize.GetHeight() - scaledH / 2 - thumbnailSize.GetHeight() / 2));
	// create a wxBitmap from the wxImage
	wxBitmap bitmap(finalImg);

	// display the wxBitmap in the wxStaticBitmap control
	m_thumbnailButton = new wxBitmapButton(this, wxID_ANY, bitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE);

	// Create video name label
	m_videoName = new wxStaticText(this, wxID_ANY, m_videoSource->GetName(), wxDefaultPosition, wxSize(thumbnailSize.GetWidth(), -1));
	m_videoName->SetForegroundColour(wxColor(240, 240, 240));

	std::string iconColor = "white";
	wxBitmap addIcon(iconColor + (std::string)"Add.png", wxBITMAP_TYPE_PNG);
	m_addButton = new SmallBitmapButton(this, wxID_ANY, addIcon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE);
	m_addButton->SetToolTip(new wxToolTip("Add to sequence"));
	// Create edit button with pencil icon
	wxBitmap editIcon(iconColor + (std::string)"Edit.png", wxBITMAP_TYPE_PNG);
	m_editButton = new SmallBitmapButton(this, wxID_ANY, editIcon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW| wxBORDER_NONE);
	m_editButton->SetToolTip(new wxToolTip("Change name"));

	// Create delete button with trashcan icon
	wxBitmap deleteIcon(iconColor + (std::string)"Delete.png", wxBITMAP_TYPE_PNG);
	m_deleteButton = new SmallBitmapButton(this, wxID_ANY, deleteIcon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE);
	m_deleteButton->SetToolTip(new wxToolTip("Delete source"));

	// Set up event handlers
	//m_thumbnailButton->Bind(wxEVT_LEFT_DCLICK, &VideoSourcePanel::onMouseLeftDoubleClick, this);
	m_thumbnailButton->Bind(wxEVT_LEFT_DOWN, wxMouseEventHandler(VideoSourcePanel::onMouseLeftDown), this);
	m_thumbnailButton->Bind(wxEVT_LEFT_DCLICK, wxMouseEventHandler(VideoSourcePanel::onMouseLeftDoubleClick), this);
	m_addButton->Bind(wxEVT_BUTTON, &VideoSourcePanel::onAddButtonClicked, this);
	m_editButton->Bind(wxEVT_BUTTON, &VideoSourcePanel::onEditButtonClicked, this);
	m_deleteButton->Bind(wxEVT_BUTTON, &VideoSourcePanel::onDeleteButtonClicked, this);
	
	wxBoxSizer* buttonsSizer_ = new wxBoxSizer(wxHORIZONTAL);
	buttonsSizer_->Add(m_addButton, 0, wxRIGHT, 5);
	buttonsSizer_->Add(m_editButton, 0, wxLEFT | wxRIGHT, 5);
	buttonsSizer_->Add(m_deleteButton, 0, wxLEFT, 5);

	m_mainSizer = new wxBoxSizer(wxVERTICAL);
	m_mainSizer->Add(m_thumbnailButton, 0, wxALL, 10);
	m_mainSizer->Add(m_videoName, 0, wxLEFT|wxRIGHT, 10);
	m_mainSizer->Add(buttonsSizer_, 0, wxALL, 10);
	m_mainSizer->Layout();
	//SetMinSize(wxSize(300, 300));
	SetSizerAndFit(m_mainSizer);
	SetBackgroundColour(wxColor(50, 50, 50));
}

VideoSourcePanel::~VideoSourcePanel()
{
	VideoSource::videoSources.RemoveRecord(m_videoSource->GetName());
	delete m_videoSource;
	//delete m_mainSizer; // deleted automatically so calling a delete will cause error while exiting
	//delete m_thumbnail;
	//delete m_addButton;
	//delete m_editButton;
	//delete m_deleteButton;
}

void VideoSourcePanel::rescaleBitmap(wxBitmap& bitmap, const wxSize& size)
{
	wxImage img = bitmap.ConvertToImage();
	img.Rescale(size.GetWidth(), size.GetHeight(), wxIMAGE_QUALITY_HIGH);
	bitmap = img;
}

void VideoSourcePanel::onMouseLeftDown(wxMouseEvent& event)
{
	wxCommandEvent event_(SHOW_VIDEO_EVENT, GetId());
	event_.SetEventObject(this);
	event_.SetString(m_videoSource->GetName());
	
	wxWindow* window = FindWindowById(m_showWindowID);
	if (window != nullptr) {
		wxPostEvent(window, event_);
	}
	
}

void VideoSourcePanel::onMouseLeftDoubleClick(wxMouseEvent& event)
{
}

void VideoSourcePanel::onAddButtonClicked(wxCommandEvent& event)
{
}

void VideoSourcePanel::onEditButtonClicked(wxCommandEvent& event)
{
	VideoSource::videoSources.UpdateName(m_videoSource->GetName());
	m_videoName->SetLabelText(m_videoSource->GetName());
	m_mainSizer->Layout();
	SetSizerAndFit(m_mainSizer);
}

void VideoSourcePanel::onDeleteButtonClicked(wxCommandEvent& event)
{
	wxWindow* topLevelParent = wxGetTopLevelParent(this);
	wxCommandEvent event_(WIDGET_DELETED_EVENT, GetId());
	event_.SetEventObject(this);
	Destroy();
	wxPostEvent(topLevelParent, event_);
}

