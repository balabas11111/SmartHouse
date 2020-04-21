package com.balabas.smarthouse.telegram.bot.message;

import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import org.apache.logging.log4j.util.Strings;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardRemove;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.service.IAlarmV2Service;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldBoolean;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.virtual.IVirtualEntityService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.entity.service.IGroupService;
import com.balabas.smarthouse.server.entity.service.IViewChartEntityFieldsService;
import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.server.view.MessageHolder;
import com.balabas.smarthouse.server.view.chart.IMetrics;
import com.balabas.smarthouse.server.view.chart.Metric;
import com.google.common.collect.Lists;
import com.google.common.collect.Sets;

import static com.balabas.smarthouse.server.view.Action.ACTION_DATA_FIELD_NAME;

import lombok.Getter;

@Component
@SuppressWarnings("rawtypes")
public class SendMessageBuilder {

	@Value("${smarthouse.server.url:#{null}}")
	private String serverUrl;

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private IVirtualEntityService virtualEntityService;

	@Autowired
	private IViewChartEntityFieldsService viewChartsService;

	@Autowired
	private IGroupService groupService;

	@Autowired
	private IAlarmV2Service alarmService2;

	@Autowired
	private InlineKeyboardBuilder inlineKeyboard;

	@Autowired
	@Getter
	private ReplyKeyboardBuilder replyKeyboard;

	@Autowired
	private EntityViewBuilder itemRendererBuilder;

	@Autowired
	private ItemTextHelper itemTextHelper;

	public List<SendMessage> createServerStartedMessages(Long chatId, String serverName) {
		List<SendMessage> msgs = Lists.newArrayList();
		String text = String.format(BotMessageConstants.BOT_REGISTERED_MSG, Emoji.PERSON_RAISING_ONE_HAND, serverName,
				new Date());
		msgs.add(createRefreshDevicesListReplyKeyboard(chatId, text));
		return msgs;
	}

	public List<SendMessage> createDeviceRegisteredMessages(String deviceName, Long chatId) {
		List<SendMessage> msgs = Lists.newArrayList();

		String text = String.format(BotMessageConstants.DEVICE_REGISTERED_MSG, Emoji.CHECK_MARK, deviceName,
				new Date());

		msgs.add(createRefreshDevicesListReplyKeyboard(chatId, text));
		return msgs;
	}

	public List<SendMessage> createMainMenuReplyKeyboard(ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();
		if(StringUtils.isEmpty(context.getText())) {
			context.setText(getServerMainText());
		}
		msgs.add(context.createMsg(replyKeyboard.getMainMenuReplyKeyboard()));

		return msgs;
	}

	public SendMessage createRefreshDevicesListReplyKeyboard(Long chatId, String text) {
		return ReplyContext.createMsg(replyKeyboard.getMainMenuReplyKeyboard(), null, chatId, text, true);
	}

	public List<Device> getDevices() {
		return deviceService.getDevicesInitialized().stream().sorted(ItemAbstract::compareByDescriptionField)
				.collect(Collectors.toList());
	}

	public List<Metric> getViewCharts() {
		return viewChartsService.getAllAsList();
	}

	public IMetrics getViewChartsAttached(Long id) {
		IMetrics metrics = viewChartsService.getById(id);

		Set<IEntityField> entityFields = metrics.getEntityFields();
		Set<IEntityField> entityFieldsResults = Sets.newLinkedHashSet();

		for (IEntityField entityField : entityFields) {
			IEntityField entityFieldResult = deviceService.getEntityFieldById(entityField.getId());
			entityFieldsResults.add(entityFieldResult);
		}

		metrics.setEntityFields(entityFieldsResults);

		return metrics;
	}
	
	public String getServerMainText() {
		List<Device> devices = getDevices();
		return getServerMainText(devices.size());
	}

	public String getServerMainText(int size) {
		String serverAddress = Optional.ofNullable(serverUrl).orElse("");

		return (size == 0) ? String.format(BotMessageConstants.NO_DEVICE_MSG, Emoji.WARNING, serverAddress)
				: String.format(BotMessageConstants.SERVER_SELECT_DEVICE_VIEW_MSG, Emoji.OUTBOX_TRAY,
						Optional.ofNullable(serverName).orElse(""), serverAddress);
	}

	public List<SendMessage> createViewOfMainCommands(Action action, ReplyContext cont) {
		List<SendMessage> msgs = Lists.newArrayList();
		List<Device> devices = getDevices();
		cont.setText(getServerMainText(devices.size()));
		msgs.add(cont.createMsg(inlineKeyboard.getInlineKeyboardViewOfMainCommands(devices)));
		return msgs;
	}

	public List<SendMessage> createViewOfDevicesVirtual(Action action, ReplyContext cont) {
		List<SendMessage> msgs = Lists.newArrayList();
		List<IDevice> devices = virtualEntityService.getDevices();

		if (devices.size() > 1) {
			List<Device> devicesObjList = devices.stream().map(d -> (Device) d).collect(Collectors.toList());
			cont.setText(getServerMainText(devices.size()));
			msgs.add(cont.createMsg(inlineKeyboard.getInlineKeyboardViewOfDevices(devicesObjList)));
		} else {
			if (devices.size() == 1) {
				action.setDeviceName(devices.get(0).getName());
				action.setGroupName(DeviceConstants.GROUP_SENSORS);
				msgs.addAll(createViewOfDeviceGroup(action, cont));
			}
		}
		return msgs;
	}

	public List<SendMessage> createViewOfDevices(Action action, ReplyContext cont) {
		List<SendMessage> msgs = Lists.newArrayList();
		List<Device> devices = getDevices();
		cont.setText(getServerMainText(devices.size()));
		msgs.add(cont.createMsg(inlineKeyboard.getInlineKeyboardViewOfDevices(devices)));
		return msgs;
	}

	public List<SendMessage> createViewOfMetrics(Action action, ReplyContext cont) {
		List<SendMessage> msgs = Lists.newArrayList();
		List<Metric> metrics = getViewCharts();
		cont.setText(getServerMainText(metrics.size()));
		msgs.add(cont.createMsg(inlineKeyboard.getInlineKeyboardViewOfMetrics(metrics)));
		return msgs;
	}
	
	public List<SendMessage> createViewOfValarmsV2(Action action, ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();
		
		ItemAbstractDto dto = ItemAbstractDto.fromShortName(action.getData());
		AlarmState maxState = AlarmState.OK;
		
		Map<ItemType, Map<AlarmState, List<IAlarmV2>>> map = dto.getItemType()!=null ?
				alarmService2.getAlarmsAsMap(dto.getId(), dto.getItemType(), maxState, false) :
				alarmService2.getAlarmsAsMap(maxState, false);
		
		StringBuilder builder = new StringBuilder();
		
		if(map==null || map.isEmpty()) {
			builder.append("Нет проверок на тревогу");
		} else {
			boolean first = true;
			
			for(ItemType it : map.keySet()) {
				if(map.containsKey(it)) {
					List<IAlarmV2> als = map.get(it).values().stream().flatMap(list -> list.stream()).collect(Collectors.toList()); 
					
					if(als!=null && als.size()>0) {
						if(!first) {
							builder.append("\n");
						}
						builder.append("<b>");
						builder.append(it.getEmojiDescription());
						builder.append("</b>");
						builder.append("\n");
						
						als.forEach(alarm -> {
							builder.append(alarm.getAlarmState().getEmoji().toString());
							builder.append(" ");
							builder.append(alarm.getDescription());
							builder.append(" ");
							builder.append(alarm.getViewDescriptor().getDescription());
							builder.append(";\n");
						});
						
						first = false;
					}
				}
			}
		}
		
		context.setText(builder.toString());
		//msgs.add(context.createMsg(inlineKeyboard.getSetupMenuKeyboard()));
		msgs.add(createHtmlMessage(context));
		return msgs;
	}

	public List<SendMessage> createViewOfEntityMetrics(Action action, ReplyContext context) {
		Long id = Long.valueOf(action.getData());
		IMetrics metrics = getViewChartsAttached(id);

		List<SendMessage> msgs = Lists.newArrayList();

		StringBuilder builder = new StringBuilder();

		if (metrics != null) {
			itemRendererBuilder.buildEntityView(metrics, builder);
		}

		context.setText(builder.toString());
		msgs.add(context.createMsg(inlineKeyboard.getInlineKeyboardViewOfMetrics(metrics)));
		return msgs;
	}

	public List<SendMessage> createSetupMenu(Action action, ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();

		context.setText(String.format(BotMessageConstants.SELECT_SETUP_ACTION, Emoji.WARNING));
		msgs.add(context.createMsg(inlineKeyboard.getSetupMenuKeyboard()));

		return msgs;
	}

	public SendMessage createDevicesListEdit(Action action, ReplyContext cont) {
		String serverName = action.getServerName();
		List<Device> devices = getDevices();

		cont.setText((devices.isEmpty()) ? String.format(BotMessageConstants.NO_DEVICE_MSG, Emoji.WARNING)
				: String.format(BotMessageConstants.SERVER_SELECT_DEVICE_EDIT_MSG, Emoji.OUTBOX_TRAY, serverName));

		return cont.createMsg(inlineKeyboard.getDevicesOfServerInlineKeyboardEdit(devices));
	}

	public SendMessage createServerWillBeRestartedMsg(ReplyContext context) {
		return createHtmlMessage(context.getChatId(), BotMessageConstants.SERVER_WILL_BE_RESTARTED_MESSAGE);
	}

	public SendMessage createHelpMsg(ReplyContext context) {
		return createHtmlMessage(context.getChatId(), BotMessageConstants.HELP_MESSAGE);
	}

	public SendMessage createGroupsOfDeviceInlineKeyboard(Action action, ReplyContext cont) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());

		cont.setText(String.format(BotMessageConstants.SELECT_GROUP_MSG, Emoji.OUTBOX_TRAY, device.getDescription(),
				device.getFirmware()));

		return cont.createMsg(inlineKeyboard.getGroupsOfDeviceInlineKeyboard(device));
	}

	public SendMessage getEntitiesOfGroupInlineKeyboard(String deviceName, String groupName, ReplyContext cont) {
		IDevice device = deviceService.getDeviceByName(deviceName);
		IGroup group = device.getGroup(groupName);

		Emoji groupEmoji = itemTextHelper.getEmojiByGroupName(groupName);
		String trans = itemTextHelper.getGroupNameTranslation(groupName);

		String text = String.format(BotMessageConstants.BUTTON, groupEmoji.toString(), trans);

		cont.setText(text);

		return cont.createMsg(inlineKeyboard.getEntitiesOfGroupInlineKeyboard(device, group));
	}

	public SendMessage getEntitiesOfDeviceToEdit(Action action, ReplyContext cont) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());

		String text = String.format(BotMessageConstants.EDIT_DEVICE_SELECT_ENTITY, Emoji.GEAR, device.getDescription());

		cont.setText(text);

		return cont.createMsg(inlineKeyboard.getEntitiesOfDeviceInlineKeyboard(device));
	}

	public SendMessage getFieldsOfEntityToEdit(Action action, ReplyContext cont) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());
		IEntity entity = device.getEntity(action.getEntityName());

		String text = String.format(BotMessageConstants.EDIT_DEVICE_SELECT_FIELD, Emoji.GEAR, device.getDescription(),
				entity.getDescription());

		cont.setText(text);

		return cont.createMsg(inlineKeyboard.getFieldsOfEntityInlineKeyboard(entity));
	}

	public SendMessage getDeviceDescriptionToEdit(Action action, ReplyContext context) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());

		String text = String.format(BotMessageConstants.EDIT_DEVICE_SELECTED_FIELD, device.getEmoji().toString(),
				device.getDescription(), DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION, "Название",
				device.getDescription());

		return createHtmlMessage(context.getChatId(), text);
	}

	public SendMessage getFieldToEdit(Action action, ReplyContext context) {
		IDevice device = deviceService.getDeviceByName(action.getDeviceName());
		IEntity entity = device.getEntity(action.getEntityName());

		String fieldName = new JSONObject(action.getData()).getString(ACTION_DATA_FIELD_NAME);

		IEntityField<?> entityField = entity.getEntityField(fieldName);

		String text = String.format(BotMessageConstants.EDIT_DEVICE_SELECTED_FIELD, device.getEmoji().toString(),
				entity.getDescription(), entityField.getName(), entityField.getDescription(),
				entityField.getValueStr());

		return createHtmlMessage(context.getChatId(), text);
	}

	// view device functionality
	public List<SendMessage> createViewOfAllDevicesGroup(Action action, ReplyContext context) {
		List<SendMessage> result = Lists.newArrayList();

		List<Device> devices = deviceService.getDevicesInitialized();

		for (Device device : devices) {
			IGroup group = device.getGroup(action.getGroupName());
			result.addAll(createViewOfDeviceGroup(device, group, context));
		}

		return result;
	}

	public List<SendMessage> createViewOfDeviceGroup(Action action, ReplyContext context) {
		List<SendMessage> result = Lists.newArrayList();

		IDevice device = deviceService.getDeviceByName(action.getDeviceName());

		IGroup group = device.getGroup(action.getGroupName());
		result.addAll(createViewOfDeviceGroup(device, group, context));

		return result;
	}

	private List<SendMessage> createViewOfDeviceGroup(IDevice device, IGroup group, ReplyContext context) {
		List<SendMessage> result = Lists.newArrayList();

		if (device != null && group != null) {

			StringBuilder builder = new StringBuilder();

			itemRendererBuilder.buildDeviceGroupHeaderView(device, group, builder);

			if (!device.isInOkState()) {
				builder.append("\n\n");
				builder.append(device.getState().getStateEmojiAndDescriptionFormatted());
			} else {
				group.getEntities().stream().sorted(ItemAbstract::compareByDescriptionField)
						.forEach(ent -> itemRendererBuilder.buildEntityView(ent, builder));

				result.add(createHtmlMessage(context.getChatId(), builder.toString()));
				group = deviceService.getGroupById(group.getId());
				group.getEntities().stream().flatMap(ent -> ent.getEntityFields().stream()).forEach(entF -> {
					if (entF.getClazz().equals(Boolean.class)) {
						EntityFieldBoolean efb = (EntityFieldBoolean) entF;
						if (efb.getTargetEntityField() != null) {
							efb.setTargetEntityField((EntityFieldBoolean) deviceService
									.getEntityFieldById(efb.getTargetEntityFieldId()));
						}
					}
				});

				Optional.ofNullable(buildGroupCommandInterface(device, group, context.getChatId()))
						.ifPresent(result::add);
			}
		}
		return result;
	}

	public static String alarmMessageHoldersToString(List<MessageHolder> alarmHolders) {
		StringBuilder builder = new StringBuilder();

		for (MessageHolder holder : alarmHolders) {
			builder.append(holder.getEmoji().toString());
			builder.append("  <b>");
			builder.append(holder.getName());
			builder.append("</b>");

			if (!StringUtils.isEmpty(holder.getStatus())) {
				builder.append(" - ");
				builder.append(holder.getStatus());
			}

			builder.append("\n\n");

			holder.getMessages().stream().forEach(message -> {
				builder.append(message);
				builder.append("\n");
			});

			builder.append("\n");
		}

		return builder.toString();
	}

	private SendMessage buildGroupCommandInterface(IDevice device, IGroup group, Long chatId) {

		List<Action> actions = groupService.getActionsForGroup(Action.ACTION_TYPE_SEND_DATA_TO_DEVICE, group);

		if (actions.isEmpty()) {
			return null;
		}

		return ReplyContext.createMsg(inlineKeyboard.getCommandButtonsByEnabledFieldCommandButtonList(actions), chatId,
				itemRendererBuilder.buildDeviceGroupHeaderCommandsView(device, group));
	}

	public SendMessage createUnknown(ReplyContext context) {
		String text = String.format(BotMessageConstants.DONT_UNDERSTAND_MSG, Emoji.FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE,
				(context.getText() == null || context.getText().isEmpty()) ? "" : context.getText());

		return createHtmlMessage(context.getChatId(), text, context.getReplyToMessageIdIfUnknown());

	}
	
	public SendMessage createHtmlMessage(ReplyContext context) {
		return createHtmlMessage(context.getChatId(), context.getText(), context.getReplyToMessageIdIfUnknown());

	}

	public SendMessage createUnauthorized(Long chatId) {
		String text = String.format(BotMessageConstants.UNAUTHORIZED_MSG, Emoji.ERROR);

		return createHtmlMessage(chatId, text);

	}

	public SendMessage createServerError(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_SERVER_ERROR, Emoji.STOP.toString(), chatId, msg);
	}

	public SendMessage createDeviceError(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_DEVICE_ERROR, Emoji.STOP.toString(), chatId, msg);
	}

	public SendMessage createDeviceRefreshed(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_DEVICE_REFRESHED, Emoji.REFRESH.toString(), chatId, msg);
	}

	public SendMessage createDataSentToDevice(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_DATA_WAS_SENT_TO_DEVICE, Emoji.PHONE_WITH_ARROW.toString(),
				chatId, msg);
	}

	public SendMessage createDeviceDataSavedOnServer(String msg, Long chatId) {
		return createMessage(BotMessageConstants.MESSAGE_DATA_WAS_SAVED_ON_SERVER, Emoji.FLOPPY.toString(), chatId,
				msg);
	}

	public SendMessage createMessage(String format, String emoji, Long chatId, String msg) {
		String text = String.format(format, emoji, (msg == null || msg.isEmpty()) ? "" : msg);
		return createHtmlMessage(chatId, text);
	}

	public SendMessage createHideReplyKeyboardMessage(Action action, ReplyContext context) {
		SendMessage sendMessage = new SendMessage();
		sendMessage.setChatId(context.getChatId());
		sendMessage.enableMarkdown(true);
		sendMessage.setReplyToMessageId(context.getMessageId());

		String text = Strings.isNotEmpty(context.getText()) ? context.getText()
				: Emoji.WAVING_HAND_SIGN.toString() +" " + BotMessageConstants.SERVER_BY_BY_ACTION;

		sendMessage.setText(text);

		ReplyKeyboardRemove replyKeyboardRemove = new ReplyKeyboardRemove();
		replyKeyboardRemove.setSelective(true);
		sendMessage.setReplyMarkup(replyKeyboardRemove);

		return sendMessage;
	}

	public SendMessage createHtmlMessage(Long chatId, String text) {
		SendMessage msg = createTextMessage(chatId, text);

		msg.enableHtml(true);

		return msg;
	}

	public SendMessage createHtmlMessage(Long chatId, String text, Integer replyToMessageId) {
		SendMessage msg = createHtmlMessage(chatId, text);
		msg.setReplyToMessageId(replyToMessageId);

		return msg;
	}

	public SendMessage createTextMessage(Long chatId, String text) {
		SendMessage msg = new SendMessage();
		msg.setChatId(chatId.toString());
		msg.setText(text);

		return msg;
	}

}
